#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "motor_control.h"

// Configuración WiFi
const char* ssid = "Micrcontroladores";
const char* password = "raspy123";

// Crear servidor WebServer en puerto 80
WebServer server(80);

// Variable de velocidad
int velocidad = 1023; // Velocidad máxima por defecto

// Página HTML con interfaz de control
const char index_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Control de Motor ESP32</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
      font-family: Arial, Helvetica, sans-serif;
      text-align: center;
    }
    h1 {
      font-size: 1.8rem;
      color: #143642;
    }
    .button {
      display: inline-block;
      background-color: #008CBA;
      border: none;
      border-radius: 4px;
      color: white;
      padding: 16px 40px;
      text-decoration: none;
      font-size: 30px;
      margin: 2px;
      cursor: pointer;
      width: 200px;
    }
    .button2 {
      background-color: #f44336;
    }
    .button3 {
      background-color: #555555;
    }
    .button4 {
      background-color: #4CAF50;
    }
    .control-container {
      margin: 20px;
    }
    .direction-container {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 10px;
      max-width: 650px;
      margin: 0 auto;
    }
    .empty {
      visibility: hidden;
    }
    .slider {
      width: 300px;
      margin: 20px auto;
    }
    .speed-display {
      font-size: 24px;
      margin: 10px;
    }
  </style>
</head>
<body>
  <h1>Control de Motor ESP32</h1>
  
  <div class="control-container">
    <h2>Control de Movimiento</h2>
    <div class="direction-container">
      <div class="empty"></div>
      <button class="button" onclick="sendCommand('avanzar')">&#8593; Adelante</button>
      <div class="empty"></div>
      
      <button class="button button4" onclick="sendCommand('izquierda')">&#8592; Izquierda</button>
      <button class="button button2" onclick="sendCommand('detener')">Detener</button>
      <button class="button button4" onclick="sendCommand('derecha')">Derecha &#8594;</button>
      
      <div class="empty"></div>
      <button class="button" onclick="sendCommand('retroceder')">&#8595; Atras</button>
      <div class="empty"></div>
    </div>
  </div>
  
  <div class="control-container">
    <h2>Control de Velocidad</h2>
    <div class="speed-display">Velocidad: <span id="speedValue">100</span>%</div>
    <input type="range" min="0" max="100" value="100" class="slider" id="speedSlider" onchange="updateSpeed(this.value)">
  </div>

  <script>
    function sendCommand(command) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/" + command, true);
      xhr.send();
    }
    
    function updateSpeed(value) {
      document.getElementById('speedValue').innerHTML = value;
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/speed?value=" + value, true);
      xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleAvanzar() {
  avanzar(velocidad);
  server.send(200, "text/plain", "Avanzando");
}

void handleRetroceder() {
  retroceder(velocidad);
  server.send(200, "text/plain", "Retrocediendo");
}

void handleDetener() {
  detener();
  server.send(200, "text/plain", "Detenido");
}

void handleIzquierda() {
  girarIzquierdaServos();
  server.send(200, "text/plain", "Girando izquierda");
}

void handleDerecha() {
  girarDerechaServos();
  server.send(200, "text/plain", "Girando derecha");
}

void handleSpeed() {
  if (server.hasArg("value")) {
    String value = server.arg("value");
    int speedPercent = value.toInt();
    velocidad = map(speedPercent, 0, 100, 0, 1023);
    server.send(200, "text/plain", "Velocidad: " + String(speedPercent) + "%");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "Pagina no encontrada");
}

void setup() {
  Serial.begin(115200);
  
  // Inicializar drivers de motor
  iniciarDrivers();
  direccionCentro(); // Centrar servos al inicio
  
  // Conectar a WiFi
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  
  // Configurar rutas del servidor web
  server.on("/", handleRoot);
  server.on("/avanzar", handleAvanzar);
  server.on("/retroceder", handleRetroceder);
  server.on("/detener", handleDetener);
  server.on("/izquierda", handleIzquierda);
  server.on("/derecha", handleDerecha);
  server.on("/speed", handleSpeed);
  server.onNotFound(handleNotFound);
  
  // Iniciar servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}
