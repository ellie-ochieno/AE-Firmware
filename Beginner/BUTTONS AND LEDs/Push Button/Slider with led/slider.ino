#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

const char* ssid = "Lewis";
const char* password = "123456789";

const int redPin = 16;
const int greenPin = 19;
const int bluePin = 33;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println(WiFi.localIP());

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<input type='range' min='0' max='255' value='" + String(redValue) + "' id='redSlider' onchange='updateSlider(\"red\", this.value)' style='width: 80%' /><br />";
    html += "<input type='range' min='0' max='255' value='" + String(greenValue) + "' id='greenSlider' onchange='updateSlider(\"green\", this.value)' style='width: 80%' /><br />";
    html += "<input type='range' min='0' max='255' value='" + String(blueValue) + "' id='blueSlider' onchange='updateSlider(\"blue\", this.value)' style='width: 80%' /><br />";
    html += "<script>"
            "function updateSlider(color, value) {"
            "  var data = {"
            "    color: color,"
            "    value: value"
            "  };"
            "  ws.send(JSON.stringify(data));"
            "}"
            "var ws = new WebSocket('ws://' + window.location.hostname + '/ws');"
            "ws.onmessage = function(event) {"
            "  var data = JSON.parse(event.data);"
            "  if (data.color === 'red') {"
            "    document.getElementById('redSlider').value = data.value;"
            "  } else if (data.color === 'green') {"
            "    document.getElementById('greenSlider').value = data.value;"
            "  } else if (data.color === 'blue') {"
            "    document.getElementById('blueSlider').value = data.value;"
            "  }"
            "}"
            "</script>";

    request->send(200, "text/html", html);
  });

  server.begin();
}

void loop() {}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");
  }
  else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  }
  else if (type == WS_EVT_DATA) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      data[len] = '\0';
      String message = String((char*)data);

      DynamicJsonDocument jsonDoc(256);
      DeserializationError error = deserializeJson(jsonDoc, message);
      if (!error) {
        String color = jsonDoc["color"].as<String>();
        int value = jsonDoc["value"].as<int>();

        if (color == "red") {
          redValue = value;
        }
        else if (color == "green") {
          greenValue = value;
        }
        else if (color == "blue") {
          blueValue = value;
        }

        updateLEDs();
      }
    }
  }
}

void updateLEDs() {
  analogWrite(redPin, map(redValue, 0, 255, 0, 1023));
  analogWrite(greenPin, map(greenValue, 0, 255, 0, 1023));
  analogWrite(bluePin, map(blueValue, 0, 255, 0, 1023));
}
