#include <WiFi.h>
#include <WebServer.h>

const char* SSID = "Hieu";
const char* PASS = "hieudeptrai34";
const int LED = 2;

WebServer server(80);
bool lit = false;

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 LED</title>
  <style>
    body { font-family: sans-serif; text-align: center; margin-top: 60px; }
    h1 { font-size: 28px; }
    #state { font-size: 48px; font-weight: bold; margin: 20px; }
    button { font-size: 24px; padding: 16px 40px; border: none;
             border-radius: 12px; background: #2196f3; color: white; }
    button:active { background: #0b79d0; }
  </style>
</head>
<body>
  <h1>Dieu khien LED</h1>
  <div id="state">...</div>
  <button onclick="toggle()">Bat / Tat</button>

  <script>
    function show(s) {
      const el = document.getElementById("state");
      el.textContent = s;
      el.style.color = (s === "ON") ? "green" : "gray";
    }
    function toggle() {
      fetch("/toggle").then(r => r.text()).then(show);
    }
    fetch("/state").then(r => r.text()).then(show);
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", PAGE);
}

void handleToggle() {
  lit = !lit;
  digitalWrite(LED, lit);
  server.send(200, "text/plain", lit ? "ON" : "OFF");
}

void handleState() {
  server.send(200, "text/plain", lit ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);

  Serial.print("Dang ket noi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.printf("\nThat bai, status = %d\n", WiFi.status());
    return;
  }

  Serial.print("\nIP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/state", handleState);
  server.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) server.handleClient();
}