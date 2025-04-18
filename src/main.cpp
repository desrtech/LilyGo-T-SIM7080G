#include <Arduino.h>
#include <PMUService.h>
#include <WiFiConnection.h>
#include <WebServerComponent.h>
#include <GPSService.h>
#include <WeatherService.h>

JSONVar weatherData;
WeatherService weatherService;

String valoresDispositivo() {
    JSONVar data;
    data["temp"] = JSON.stringify( weatherData["datos"]["valoresMasRecientes"]["temperatura"]);
    Serial.println(weatherData["datos"]["valoresMasRecientes"]["temperatura"]);

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

    // String url = "https://api.open-meteo.com/v1/forecast?latitude=-33.127524858853434&longitude=-71.56444467716497&current_weather=true";
    String url = "https://climatologia.meteochile.gob.cl/application/servicios/getEmaResumenDiario/330007?usuario=dsilva.david@gmail.com&token=ddd60131dbfce73612ebc9ee";
    weatherData = weatherService.fetchWeatherData(url);
    // Serial.println("Weather data fetched: " + JSON.stringify(weatherData));
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
