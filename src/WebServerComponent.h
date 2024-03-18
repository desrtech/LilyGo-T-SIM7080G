#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <SPIFFS.h>
#include <Arduino_JSON.h>

AsyncWebServer server(8090);
WebSocketsServer webSockets(81);

JSONVar dataJson;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Create an Event Source on /events
AsyncEventSource events("/events");

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "plain/text", "Pagina no encontrada");
}

String getJsonData(String systemVoltage, String bateryVoltage) {
    dataJson["systemVoltage"] = systemVoltage;
    dataJson["bateryVoltage"] = bateryVoltage;
    String json = JSON.stringify(dataJson);
    return json;
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

    events.onConnect([](AsyncEventSourceClient *client){
        if(client->lastId()){
            Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
        }
        // send event with message "hello!", id current millis
        // and set reconnect delay to 1 second
        client->send("hello!", NULL, millis(), 10000);
    });

    server.addHandler(&events);

    server.onNotFound(notFound);
    server.begin();
}

void loopGetDataJson(String systemVoltage, String bateryVoltage) {
    if ((millis() - lastTime) > 3000) {
        // events.send("ping", NULL, millis());
        events.send(getJsonData(systemVoltage, bateryVoltage).c_str(), "new_readings" ,millis());
        lastTime = millis();
    }
}

