#include <Arduino.h>
#include <Ticker.h>
// #include <utilities.h>
#include <PMUService.h>
#include <WiFiConnection.h>
// #include <WebServerComponent.h>

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
    chargerIrqStatusTimer.attach(5, irqStatus);
    powerStatusTimer.attach(5, powerStatus);
    iniciaWifiManager();
    ipAddressTimer.attach(5, ipAddress);
}

void loop()
{
    
}
