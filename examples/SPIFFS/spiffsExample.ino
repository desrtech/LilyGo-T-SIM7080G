#include <Arduino.h>
#include <SPIFFS.h>

#include <Ticker.h>

void setup() {
    Serial.begin(115200);
    while(!Serial);
    delay(500);
    Serial.println("Iniciado Sistema de archivos");
    montarFileSystem();
    delay(500);
    Serial.println("Leyendo archivos");
    listarArchivos();
}

void loop() {

}

void montarFileSystem() {
    Serial.println("Montando sistema de archivos");
    if(!SPIFFS.begin()){
        Serial.println("Error al montar sistema de archivos");
    } else {
        Serial.println("Sistema de archivos montado");
    }
}

void listarArchivos() {
    Serial.println("Listar archivos");
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    if (!file) {
        Serial.println("No existen archivos");
    }
    
    int count = 0;
    while(file) {
        Serial.print(count + "- ");
        Serial.print("Nombre Archivo: ");
        Serial.print(file.name());
        Serial.print(" size: ");
        Serial.println(file.size());
        file = root.openNextFile();
    }

}

void formatarFileSystem() {
    Serial.println("Formateamdo sistema de archivos");
    #if defined(ESP32)
        if(!SPIFFS.begin(true)) {
            Serial.println("Error montando partición");
        }
    #endif

    if(!SPIFFS.format()) {
        Serial.println("Error formateando sistema de archivos");
    } else {
        Serial.println("Formato completado");
    }
}

void leerArchivo(String ruta) {
    File file = SPIFFS.open(ruta);
    if(!file){
        Serial.println("Error al leer archivo");
        return;
    }

    while(file.available()){
        Serial.write(file.read());
    }

    file.close();
}