# AnemoSense
Dispositivo IoT de calidad de aire en espacios cerrados.
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">


<h1>📌 Pinout</h1>

<h2>BME680 - Comunicación I²C</h2>

<table>

<tr>
    <th>BME680</th>
    <th>ESP32</th>
</tr>

<tr>
    <td>VDD</td>
    <td>3.3V</td>
</tr>

<tr>
    <td>GND</td>
    <td>GND</td>
</tr>

<tr>
    <td>VDDIO</td>
    <td>3.3V</td>
</tr>

<tr>
    <td>SDO</td>
    <td>3.3V</td>
</tr>

<tr>
    <td>CSB</td>
    <td>3.3V</td>
</tr>

<tr>
    <td>SDI</td>
    <td>GPIO21</td>
</tr>

<tr>
    <td>SCL</td>
    <td>GPIO22</td>
</tr>

</table>

<h2>SCD41 - Comunicación I²C</h2>

<table>

<tr>
    <th>SCD41</th>
    <th>ESP32</th>
</tr>

<tr>
    <td>VDD</td>
    <td>3.3V</td>
</tr>

<tr>
    <td>VDDH</td>
    <td>3.3V</td>
</tr>

<tr>
    <td>GND</td>
    <td>GND</td>
</tr>

<tr>
    <td>SDA</td>
    <td>GPIO21</td>
</tr>

<tr>
    <td>SCL</td>
    <td>GPIO22</td>
</tr>

</table>

<h2>GC9A01 - Comunicación SPI</h2>

<table>

<tr>
    <th>SCD41</th>
    <th>ESP32</th>
</tr>

<tr>
    <th>RST</th>
    <th>GPIO4</th>
</tr>

<tr>
    <th>CS</th>
    <th>GPIO5</th>
</tr>

<tr>
    <th>DC</th>
    <th>GPIO2</th>
</tr>

<tr>
    <th>SDA</th>
    <th>GPIO23</th>
</tr>

<tr>
    <th>SCL</th>
    <th>GPIO18</th>
</tr>

<tr>
    <th>GND</th>
    <th>GND</th>
</tr>

<tr>
    <th>VCC</th>
    <th>3.3V</th>
</tr>


</table>

</body>
</html>
