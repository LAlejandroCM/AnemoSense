#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <SensirionI2cScd4x.h>

//==================== WIFI ====================
const char* ssid = "";          //Nombre de red WIFI
const char* password = "";      //Constraseña de red WIFI
//==================== MQTT ====================
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

//==================== TFT ====================
#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4

Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_RST);

// Paleta de colores
#define COLOR_BG       0x0000 // Negro
#define COLOR_TEXT     0xFFFF // Blanco
#define COLOR_TEMP     0xFD20 // Naranja
#define COLOR_HUM      0x05FF // Cian
#define COLOR_PRES     0xFFE0 // Amarillo
#define COLOR_CO2      0xF81F // Magenta
#define COLOR_ERROR    0xF800 // Rojo brillante
#define COLOR_OK       0x07E0 // Verde brillante
#define COLOR_ANEMO    0x7615

//==================== I2C ====================
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_BME680 bme;
SensirionI2cScd4x scd4x;

// Flags de estado de los sensores
bool bmePresente = false;
bool scdPresente = false;

//==================== VARIABLES ====================
float temperatura = 0;
float humedad = 0;
float presion = 0;

uint16_t co2 = 0;
float scdTemp = 0;
float scdHum = 0;

// Variables de tiempo para control asíncrono (No bloqueante)
unsigned long ultimoIntentoConexion = 0;
const unsigned long intervaloConexion = 10000; // Reintentar red cada 10 segundos

// NUEVOS INTERVALOS SOLICITADOS
unsigned long ultimaLecturaBME = 0;
const unsigned long intervaloBME = 3000; // Leer BME680 cada 3 segundos

unsigned long ultimaActualizacionTFT = 0;
const unsigned long intervaloTFT = 2000; // Actualizar pantalla cada 2 segundos

unsigned long ultimoEnvioMQTT = 0;
const unsigned long intervaloMQTT = 10000; // Publicar por MQTT cada 10 segundos


//==================== FUNCIONES DE CONECTIVIDAD NO BLOQUEANTES ====================
void verificarConectividad() {
  unsigned long tiempoActual = millis();

  // Si no hay WiFi, intentamos conectar asíncronamente
  if (WiFi.status() != WL_CONNECTED) {
    if (tiempoActual - ultimoIntentoConexion >= intervaloConexion) {
      ultimoIntentoConexion = tiempoActual;
      Serial.println("Intentando conectar a WiFi...");
      WiFi.disconnect();
      WiFi.begin(ssid, password);
    }
  } 
  // Si hay WiFi pero MQTT está desconectado, intentamos conectar MQTT
  else if (!client.connected()) {
    if (tiempoActual - ultimoIntentoConexion >= intervaloConexion) {
      ultimoIntentoConexion = tiempoActual;
      Serial.println("Intentando conectar a Broker MQTT...");
      String clientId = "ESP32-";
      clientId += String(random(0xffff), HEX);
      
      if (client.connect(clientId.c_str())) {
        Serial.println("MQTT Conectado de forma asíncrona");
      } else {
        Serial.print("Fallo MQTT, estado: ");
        Serial.println(client.state());
      }
    }
  }
}

//==================== SETUP ====================
void setup() {
  delay(500);

  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  // Inicializar TFT con secuencia de Reset por Hardware
  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, LOW);
  delay(100);
  digitalWrite(TFT_RST, HIGH);
  delay(100);

  tft.begin();
  tft.setRotation(2); 
  tft.fillScreen(COLOR_BG);

  // Iniciar WiFi
  WiFi.begin(ssid, password);
  client.setServer(mqtt_server, 1883);

  // Inicializar BME680
  if (bme.begin(0x77)) {
    bmePresente = true;
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
  } else {
    Serial.println("Error: BME680 no encontrado.");
  }

  // Inicializar SCD41
  scd4x.begin(Wire, SCD41_I2C_ADDR_62);
  uint16_t error = scd4x.stopPeriodicMeasurement();
  if (error == 0) {
    scdPresente = true;
    scd4x.startPeriodicMeasurement();
  } else {
    Serial.println("Error: SCD41 no encontrado.");
  }

  delay(2000);
}

//==================== LOOP ====================
void loop() {
  // Manejo de la pila MQTT (solo si está conectado)
  if (client.connected()) {
    client.loop();
  }

  // Verifica y gestiona las reconexiones en segundo plano de forma no bloqueante
  verificarConectividad();

  unsigned long tiempoActual = millis();

  //========== LECTURA BME680 (Cada 3 segundos) ==========
  if (bmePresente && (tiempoActual - ultimaLecturaBME >= intervaloBME)) {
    ultimaLecturaBME = tiempoActual;
    if (bme.performReading()) {
      temperatura = bme.temperature;
      humedad = bme.humidity;
      presion = bme.pressure / 100.0;
    } else {
      temperatura = 0;
      humedad = 0;
      presion = 0;
    }
  }

  //========== LECTURA SCD41 (Cuando hay medición nueva, sin temporizador) ==========
  if (scdPresente) {
    bool dataReady = false;
    scd4x.getDataReadyStatus(dataReady);
    if (dataReady) {
      scd4x.readMeasurement(co2, scdTemp, scdHum);
      Serial.print("Nueva lectura SCD41 - CO2: ");
      Serial.println(co2);
    }
  } else {
    co2 = 0;
  }

  //========== PUBLICAR POR MQTT (Cada 10 segundos) ==========
  if (client.connected() && (tiempoActual - ultimoEnvioMQTT >= intervaloMQTT)) {
    ultimoEnvioMQTT = tiempoActual;
    char buffer[32];
    
    dtostrf(temperatura, 1, 2, buffer);
    client.publish("DYCDE/AnemoSense/temp", buffer);

    dtostrf(humedad, 1, 2, buffer);
    client.publish("DYCDE/AnemoSense/hum", buffer);

    dtostrf(presion, 1, 2, buffer);
    client.publish("DYCDE/AnemoSense/pres", buffer);

    sprintf(buffer, "%u", co2);
    client.publish("DYCDE/AnemoSense/co2", buffer);
    
    Serial.println("Datos publicados en MQTT.");
  }

  //========== RENDERIZADO EN PANTALLA TFT (Cada 2 segundos) ==========
  if (tiempoActual - ultimaActualizacionTFT >= intervaloTFT) {
    ultimaActualizacionTFT = tiempoActual;

    tft.fillScreen(COLOR_BG);

    // --- Encabezado / Título ---
    tft.setTextSize(2);
    tft.setTextColor(COLOR_ANEMO);
    tft.setCursor(60, 30);
    tft.print("AnemoSense");
    
    tft.drawFastHLine(40, 52, 160, COLOR_TEXT);

    // --- Bloque 1: Temperatura ---
    tft.setCursor(45, 65);
    if (bmePresente) {
      tft.setTextColor(COLOR_TEMP);
      tft.printf("TEMP: %.1f C", temperatura);
    } else {
      tft.setTextColor(COLOR_ERROR);
      tft.print("BME680 OFF");
    }

    // --- Bloque 2: Humedad ---
    tft.setCursor(45, 95);
    if (bmePresente) {
      tft.setTextColor(COLOR_HUM);
      tft.printf("HUM:  %.1f %%", humedad);
    } else {
      tft.setTextColor(COLOR_ERROR);
      tft.print("BME680 OFF");
    }

    // --- Bloque 3: Presión ---
    tft.setCursor(45, 125);
    if (bmePresente) {
      tft.setTextColor(COLOR_PRES);
      tft.printf("PRES: %.0f hPa", presion);
    } else {
      tft.setTextColor(COLOR_ERROR);
      tft.print("BME680 OFF");
    }

    tft.drawFastHLine(40, 155, 160, COLOR_TEXT);

    // --- Bloque 4: CO2 ---
    tft.setCursor(45, 170);
    if (scdPresente) {
      tft.setTextColor(COLOR_CO2);
      tft.printf("CO2:  %u ppm", co2);
    } else {
      tft.setTextColor(COLOR_ERROR);
      tft.print("SCD41 OFF");
    }

    // --- Barra de Estado Conectividad (WIFI / MQTT dinámico) ---
    tft.setTextSize(1);
    
    // Imprimir Estado WIFI
    tft.setCursor(85, 205);
    if (WiFi.status() == WL_CONNECTED) {
      tft.setTextColor(COLOR_OK);
    } else {
      tft.setTextColor(0x7BEF);
    }
    tft.print("WIFI");

    // Imprimir Estado MQTT
    tft.setCursor(130, 205);
    if (client.connected()) {
      tft.setTextColor(COLOR_OK);
    } else {
      tft.setTextColor(0x7BEF);
    }
    tft.print("MQTT");
  }
}
