#include <Arduino.h>
#include <LittleFS.h>

void montarSystemFile() {
    Serial.println("Montando sistema de archivos");
    if(!LittleFS.begin()){
        Serial.println("Error al montar sistema de archivos");
    } else {
        Serial.println("Sistema de archivos montado");
    }
}

void listarArchivos() {
    Serial.println("Listar archivos");
    File root = LittleFS.open("/");
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
        if(!LittleFS.begin(true)) {
            Serial.println("Error montando partición");
        }
    #endif

    if(!LittleFS.format()) {
        Serial.println("Error formateando sistema de archivos");
    } else {
        Serial.println("Formato completado");
    }
}

void leerArchivo(String ruta) {
    File file = LittleFS.open(ruta);
    if(!file){
        Serial.println("Error al leer archivo");
        return;
    }

    while(file.available()){
        Serial.write(file.read());
    }

    file.close();
}