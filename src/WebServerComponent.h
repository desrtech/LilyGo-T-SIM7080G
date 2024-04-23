#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <SPIFFS.h>
#include <Arduino_JSON.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

JSONVar dataJson;

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

void notifyClients() {
    // Serial.println("Notifica a los clientes");
    ws.textAll(JSON.stringify(dataJson));
}

void sendClientMessage(String data) {
    ws.textAll(data);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      String sensorReadings = JSON.stringify(dataJson);
      notifyClients();
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
 void *arg, uint8_t *data, size_t len) {
    Serial.println("onEvent Function");
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            Serial.println("Recibiendo datos desde web");
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void initWebSocket() {
    Serial.println("InitWebSocket Function");
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void notFound(AsyncWebServerRequest *request) {
    Serial.println("Not found message");
    request->send(404, "plain/text", "Pagina no encontrada");
}

void iniciaWebServer() {
    Serial.println("Iniciando WebServer");
    if(SPIFFS.begin(true)) {
        Serial.println("Se ha montado SPIFFS");
        // Listar Archivos
        File root = SPIFFS.open("/");
        File file = root.openNextFile();
        while (file) {
            Serial.print("Archivo: ");
            Serial.println(file.name());
            file = root.openNextFile();
        }
    } else {
        Serial.println("Error al montar SPIFFS");
    }

    initWebSocket();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/index.html", "text/html");
    });

    server.on("css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "css/styles.css", "text/css");
    });

    server.on("js/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "js/script.js", "application/javascript");
    });

    server.on("img/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "img/favicon.ico", "image/x-icon");
    });

    server.on("css/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "css/bootstrap.min.css", "text/css");
    });

    server.on("js/jquery-3.4.1.slim.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "js/jquery-3.4.1.slim.min.js", "application/javascript");
    });

    server.on("js/popper.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "js/popper.min.js", "application/javascript");
    });

    server.on("js/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "js/bootstrap.min.js", "application/javascript");
    });

    server.on("js/gauge.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "js/gauge.min.js", "application/javascript");
    });

    server.on("js/leaflet.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "js/leaflet.js", "application/javascript");
    });

    server.on("css/leaflet.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "css/leaflet.css", "text/css");
    });

    server.on("js/images/marker-icon.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "js/images/marker-icon.png", "image/png");
    });

    server.on("js/images/marker-shadow.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "js/images/marker-shadow.png", "image/png");
    });

    server.serveStatic("/", SPIFFS, "/");

    server.onNotFound(notFound);
    server.begin();
}


