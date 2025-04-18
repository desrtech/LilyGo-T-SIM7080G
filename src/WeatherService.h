#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
// #include <ArduinoJson.h>
#include <Arduino_JSON.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>


class WeatherService {

    HTTPClient http;
public:
    JSONVar fetchWeatherData(const String& url) {
        JSONVar jsonData;

        http.begin(url);
        int httpCode = http.GET();

        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                jsonData = JSON.parse(payload);

                if (JSON.typeof(jsonData) == "undefined") {
                    Serial.println("JSON parsing failed.");
                }
            } else {
                Serial.print("HTTP request failed with code: ");
                Serial.println(httpCode);
            }
        } else {
            Serial.print("HTTP connection failed: ");
            Serial.println(http.errorToString(httpCode).c_str());
        }

        http.end();
        return jsonData;
    }
};
