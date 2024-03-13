#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <SPIFFS.h>
#include <Arduino_JSON.h>

AsyncWebServer server(8090);
WebSocketsServer webSockets(81);

JSONVar dataJson;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "plain/text", "Pagina no encontrada");
}

String getJsonData(String systemVoltage) {
    dataJson["systemVoltage"] = String(systemVoltage);

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

    server.onNotFound(notFound);
    server.begin();
}



