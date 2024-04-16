#include <Arduino.h>
#include "utilities.h"
#include <Arduino_JSON.h>

#define DUMP_AT_COMMANDS
#define TINY_GSM_RX_BUFFER 1024
#define TINY_GSM_MODEM_SIM7080
#include <TinyGsmClient.h>
#include "utilities.h"

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(Serial1, Serial);
TinyGsm        modem(debugger);
#else
TinyGsm        modem(SerialAT);
#endif

JSONVar gpsDataJson;

float lat2      = 0;
float lon2      = 0;
float speed2    = 0;
float alt2      = 0;
int   vsat2     = 0;
int   usat2     = 0;
float accuracy2 = 0;
int   year2     = 0;
int   month2    = 0;
int   day2      = 0;
int   hour2     = 0;
int   min2      = 0;
int   sec2      = 0;
bool  level     = false;

void iniciaGps() {
    delay(3000);
    Serial.println("Inicializando GPS");

    Serial1.begin(112500, SERIAL_8N1, BOARD_MODEM_RXD_PIN, BOARD_MODEM_TXD_PIN);
    pinMode(BOARD_MODEM_PWR_PIN, OUTPUT);

    digitalWrite(BOARD_MODEM_PWR_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_MODEM_PWR_PIN, HIGH);
    delay(1000);
    digitalWrite(BOARD_MODEM_PWR_PIN, LOW);
    
    int retry = 0;
    while(!modem.testAT(1000)) {
        Serial.print(".");
        if (retry++ > 15) {
            // Pull down PWRKEY for more than 1 second according to manual requirements
            digitalWrite(BOARD_MODEM_PWR_PIN, LOW);
            delay(100);
            digitalWrite(BOARD_MODEM_PWR_PIN, HIGH);
            delay(1000);
            digitalWrite(BOARD_MODEM_PWR_PIN, LOW);
            retry = 0;
            Serial.println("Retry start modem .");
        }
    }
    Serial.println();
    Serial.print("GPS Started");

    modem.disableGPS();
    delay(500);


    #if 0
        /*
        ! GNSS Work Mode Set
        <gps mode> GPS work mode.
            1 Start GPS NMEA out.
        <glo mode> GLONASS work mode.
            0 Stop GLONASS NMEA out.
            1 Start GLONASS NMEA out.
        <bd mode> BEIDOU work mode.
            0 Stop BEIDOU NMEA out.
            1 Start BEIDOU NMEA out.
        <gal mode> GALILEAN work mode.
            0 Stop GALILEAN NMEA out.
            1 Start GALILEAN NMEA out.
        <qzss mode> QZSS work mode.
            0 Stop QZSS NMEA out.
            1 Start QZSS NMEA out.*/

        //GNSS Work Mode Set GPS+BEIDOU
        modem.sendAT("+CGNSMOD=1,1,0,0,0");
        modem.waitResponse();
        /*
        GNSS Command,For more parameters, see <SIM7070_SIM7080_SIM7090 Series_AT Command Manual> 212 page.
        <minInterval> range: 1000-60000 ms
        minInterval is the minimum time interval in milliseconds that must elapse between position reports. default value is 1000.
        <minDistance> range: 0-1000
        Minimum distance in meters that must be traversed between position reports. Setting this interval to 0 will be a pure time-based tracking/batching.
        <accuracy>:
            0  Accuracy is not specified, use default.
            1  Low Accuracy for location is acceptable.
            2 Medium Accuracy for location is acceptable.
            3 Only High Accuracy for location is acceptable.
        */
        // minInterval = 1000,minDistance = 0,accuracy = 0
        modem.sendAT("+SGNSCMD=2,1000,0,0");
        modem.waitResponse();

        // Turn off GNSS.
        modem.sendAT("+SGNSCMD=0");
        modem.waitResponse();
    #endif
    delay(500);
    // GPS function needs to be enabled for the first use
    if (modem.enableGPS() == false) {
        Serial.print("Modem enable gps function failed!!");
        while (1) {
            delay(5000);
        }
    }
}

String getGpsPositionData() {
        String gpsPositionData = "";
        if (modem.getGPS(&lat2, &lon2, &speed2, &alt2, &vsat2, &usat2, &accuracy2,
                     &year2, &month2, &day2, &hour2, &min2, &sec2)) {
            gpsDataJson["latitud"] = String(lat2, 8);
            gpsDataJson["longitud"] = String(lon2, 8);
            gpsDataJson["velocidad"] = speed2;
            gpsDataJson["altitud"] = alt2;
            gpsDataJson["agno"] = year2;
            gpsDataJson["mes"] = month2;
            gpsDataJson["dia"] = day2;
            gpsDataJson["hora"] = hour2;
            gpsDataJson["minuto"] = min2;
            gpsDataJson["segundo"] = sec2;
            gpsPositionData = JSON.stringify(gpsDataJson);
        }
        return gpsPositionData;
    }