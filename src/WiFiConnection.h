#include <WiFi.h>
#include <WiFiManager.h>

WiFiManager wm;
bool resWm;
WiFiClass wf;

void iniciaWifiManager() {
    Serial.println("Iniciando WifiManager");
    wf.mode(WIFI_STA);
    
    
    resWm = wm.autoConnect("AutoConnectAP", "D0v2d@5481");
    if(!resWm){
        Serial.println("Failed to connect");
    } else {
        Serial.println("Connected ... yeey :)");
        Serial.println("Ip Address");
        Serial.println(wf.localIP());
        delay(2000);
    }
}

void ipAddress() {
    Serial.print("Ip Address: ");
    Serial.print(wf.localIP());
    Serial.print("  DNS IP: ");
    Serial.print(wf.dnsIP());
    Serial.print("  GW IP: ");
    Serial.print(wf.gatewayIP());
    Serial.print("  MAC AD: ");
    Serial.println(wf.macAddress());
}