#include <Arduino.h>
#include <PMUService.h>
#include <WiFiConnection.h>
#include <WebServerComponent.h>
#include <GPSService.h>

String valoresDispositivo() {
    JSONVar data;
    data["system_voltage"] = PMU.getSystemVoltage();
    data["batt_voltage"] = PMU.getBattVoltage();
    data["ip_address"] = wf.localIP().toString();
    data["gpsData"] = getGpsPositionData() != "" ? getGpsPositionData() : "{\"latitud\":\"-33.127524858853434\",\"longitud\":\"-71.56444467716497\"}";
    return JSON.stringify(data);
}

void setup()
{
    Serial.begin(115200);
    
    delay(10000);
    // while(!Serial);
    Serial.println("Serial connected");
    iniciaXPowers();
    iniciaWifiManager();
    
    while(!resWm);
    iniciaWebServer();
    iniciaGps();

    server.on("/dataJson", HTTP_GET, [](AsyncWebServerRequest *request) {
        String json = valoresDispositivo();
        request->send(200, "application/json", json);
        json = String();
    });


    PMU.setChargingLedMode(XPOWERS_CHG_LED_BLINK_1HZ);

}

uint32_t cadaMedioSegundo;
uint32_t cadaSegundo;
uint32_t cadaTreintaSegundos;

void loop()
{
    
    if ((millis() - cadaMedioSegundo) > 500) {
        // enviaLecturas();
        cadaMedioSegundo = millis();
    }


    
    if ((millis() - cadaSegundo) > 1000) {
        sendClientMessage(valoresDispositivo());
        irqStatus();
        cadaSegundo = millis();
    }

    
    if ((millis() - cadaTreintaSegundos) > 30000) {
        powerStatus();
        ipAddress();
        cadaTreintaSegundos = millis();
    }

    ws.cleanupClients();
}
