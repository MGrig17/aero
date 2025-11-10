#include "AngleSensor.h"

AngleSensor angleSensor;

AngleSensor::AngleSensor() : magnetDetected(false) {
}

void AngleSensor::begin() {
    Wire.begin();
    
    if(ams5600.detectMagnet() == 0) {
        Serial.println("Magnet not detected!");
        magnetDetected = false;
        return;
    }
    
    magnetDetected = true;
    Serial.print("Magnet detected. Current Magnitude: ");
    Serial.println(ams5600.getMagnitude());
}

float AngleSensor::getAngle() {
    if (!magnetDetected) return -1.0;
    
    word rawAngle = ams5600.getRawAngle();
    float angle = rawAngle * 0.087890625; // Конвертация в градусы
    
    // Отладочный вывод
    Serial.print("Raw angle: ");
    Serial.print(rawAngle);
    Serial.print(", Converted angle: ");
    Serial.println(angle);
    
    return angle;
}

bool AngleSensor::isMagnetDetected() {
    return magnetDetected;
}