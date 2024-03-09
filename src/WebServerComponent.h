#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketServer.h>

AsyncWebServer server(8090);
WebSocketServer webSocket();

