#include <Arduino.h>
#include <utilities.h>
#include <PMUService.h>
#include <WiFiConnection.h>
#include <WebServerComponent.h>

void setup()
{
    Serial.begin(115200);
    //Start while waiting for Serial monitoring
    while (!Serial);
    delay(3000);
    Serial.println("Serial connected");
    iniciaWifiManager();
    iniciaXPowers();
    // iniciaWebServer();
}

void loop()
{
    pmuCharging();
    // server.handleClient();
}
