#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <SPIFFS.h>

AsyncWebServer server(8090);
WebSocketsServer webSockets(81);

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

    server.on("/css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/styles.css", "text/css");
    });

    server.on("/img/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/img/favicon.ico", "image/x-icon");
    });

    server.onNotFound(notFound);
    server.begin();
}



