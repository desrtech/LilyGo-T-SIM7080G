#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <SPIFFS.h>
#include <Arduino_JSON.h>

AsyncWebServer server(8090);
AsyncWebSocket ws("/ws");

JSONVar dataJson;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Create an Event Source on /events
AsyncEventSource events("/events");

void notifyClients() {
    ws.textAll(JSON.stringify(dataJson));
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
 void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
        Serial.println("Recibiendo datos desde web");
    //   handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}



void notFound(AsyncWebServerRequest *request) {
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

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/index.html", "text/html");
    });

    server.serveStatic("/", SPIFFS, "/");

    server.on("/css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/styles.css", "text/css");
    });

    server.on("/js/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/script.js", "application/javascript");
    });

    server.on("/img/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/img/favicon.ico", "image/x-icon");
    });

    // server.on("/dataJson", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     String json = getJsonData();
    //     request->send(200, "application/json", json);
    //     json = String();
    // });

    // events.onConnect([](AsyncEventSourceClient *client){
    //     if(client->lastId()){
    //         Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    //     }
    //     // send event with message "hello!", id current millis
    //     // and set reconnect delay to 1 second
    //     client->send("hello!", NULL, millis(), 10000);
    // });

    // server.addHandler(&events);

    server.onNotFound(notFound);
    server.begin();
}

// void loopGetDataJson(String systemVoltage) {
//     if ((millis() - lastTime) > timerDelay) {
//         Serial.print("Envía datos cada 10s: ");
//         Serial.println(getJsonData(systemVoltage).c_str());
//         // events.send("ping", NULL, millis());
//         events.send(getJsonData(systemVoltage).c_str(),"new_reading" ,millis());
//         lastTime = millis();
//     }
// }

