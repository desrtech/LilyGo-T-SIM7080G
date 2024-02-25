#include <WiFiManager.h>

void iniciaWifiManager() {
    Serial.println("Iniciando WifiManager");
    WiFi.mode(WIFI_STA);
    WiFiManager wm;
    bool resWm;
    resWm = wm.autoConnect("AutoConnectAP", "D0v2d@5481");
    if(!resWm){
        Serial.println("Failed to connect");
    } else {
        Serial.println("Connected ... yeey :)");
        Serial.println("Ip Address");
        Serial.println(WiFi.localIP());
        delay(2000);
    }
}