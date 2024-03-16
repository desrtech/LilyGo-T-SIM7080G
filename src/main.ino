#include <Arduino.h>
#include <Ticker.h>
// #include <utilities.h>
#include <PMUService.h>
#include <WiFiConnection.h>
#include <WebServerComponent.h>

Ticker chargeLedBlinkTimer;
Ticker chargerIrqStatusTimer;
Ticker powerStatusTimer;
Ticker ipAddressTimer;

void setup()
{
    Serial.begin(115200);
    while (!Serial);
    delay(3000);
    Serial.println("Serial connected");

    iniciaXPowers();
    PMU.setChargingLedMode(XPOWERS_CHG_LED_BLINK_1HZ);
    chargerIrqStatusTimer.attach(1, irqStatus);
    powerStatusTimer.attach(30, powerStatus);

    iniciaWifiManager();
    ipAddressTimer.attach(30, ipAddress);
    while(!resWm);
    iniciaWebServer();

    server.on("/dataJson", HTTP_GET, [](AsyncWebServerRequest *request) {
        String json = getJsonData(String(PMU.getSystemVoltage()));
        request->send(200, "application/json", json);
        json = String();
    });

}

void loop()
{
    loopGetDataJson(String(PMU.getSystemVoltage()));
}
