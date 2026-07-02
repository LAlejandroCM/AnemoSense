# AnemoSense
Dispositivo IoT de calidad de aire en espacios cerrados.
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Pinout Sensores</title>

<style>
body{
    background:#0d1117;
    color:#f0f6fc;
    font-family:Arial, Helvetica, sans-serif;
    margin:40px;
}

h1{
    border-bottom:1px solid #30363d;
    padding-bottom:10px;
}

h2{
    margin-top:35px;
    color:#ffffff;
}

table{
    border-collapse:collapse;
    margin-top:15px;
    margin-bottom:25px;
}

th,td{
    border:1px solid #8b949e;
    padding:10px 18px;
    text-align:center;
}

th{
    background:#161b22;
}

tr:nth-child(even){
    background:#161b22;
}

.container{
    display:flex;
    gap:40px;
    flex-wrap:wrap;
}
</style>

</head>
<body>

<h1>📌 Pinout</h1>

<h2>AnemoSense</h2>

<table>
<tr>
    <th>Señal</th>
    <th>PIN</th>
</tr>

<tr>
    <td>VCC</td>
    <td>3.3V</td>
</tr>

<tr>
    <td>GND</td>
    <td>GND</td>
</tr>

<tr>
    <td>TX</td>
    <td>RX2 (GPIO16)</td>
</tr>

<tr>
    <td>RX</td>
    <td>TX2 (GPIO17)</td>
</tr>

</table>



<h2>BME680 - Comunicación I²C</h2>

<table>

<tr>
    <th>BME680</th>
    <th>ESP32</th>
</tr>

<tr>
    <td>VIN</td>
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



<h2>SCD41 - Comunicación I²C</h2>

<table>

<tr>
    <th>SCD41</th>
    <th>ESP32</th>
</tr>

<tr>
    <td>VIN</td>
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

</body>
</html>
