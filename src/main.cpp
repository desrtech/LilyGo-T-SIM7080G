#include <Arduino.h>
#include <Ticker.h>
#include <PMUService.h>
#include <WiFiConnection.h>
#include <WebServerComponent.h>

Ticker chargeLedBlinkTimer;
Ticker chargerIrqStatusTimer;
Ticker powerStatusTimer;
Ticker ipAddressTimer;
Ticker actualizacionJsonData;


void iniciaValores() {
    
    dataJson["system_voltage"] = PMU.getSystemVoltage();
    dataJson["batt_voltage"] = PMU.getBattVoltage();
    dataJson["ip_address"] = wf.localIP().toString();
    Serial.print("IniciaValores: "); Serial.println(dataJson["ip_address"]);
}

void enviaLecturas() {
    Serial.println("Envía lecturas");
    iniciaValores();
    Serial.print(JSON.stringify(dataJson));
    notifyClients();
}

void setup()
{
    Serial.begin(115200);
    while (!Serial);
    delay(3000);
    Serial.println("Serial connected");

    iniciaXPowers();
    
    chargerIrqStatusTimer.attach(1, irqStatus);
    powerStatusTimer.attach(30, powerStatus);

    iniciaWifiManager();
    ipAddressTimer.attach(30, ipAddress);
    while(!resWm);

    iniciaWebServer();

    iniciaValores();

    server.on("/dataJson", HTTP_GET, [](AsyncWebServerRequest *request) {
        String json = JSON.stringify(dataJson);
        request->send(200, "application/json", json);
        json = String();
    });


    PMU.setChargingLedMode(XPOWERS_CHG_LED_BLINK_1HZ);

    actualizacionJsonData.attach_ms(500, enviaLecturas);

}

void loop()
{
    ws.cleanupClients();
}
