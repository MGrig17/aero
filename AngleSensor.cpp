#include "AngleSensor.h"

AngleSensor angleSensor;

AngleSensor::AngleSensor() : magnetDetected(false) {
}

void AngleSensor::begin() {
    Wire.begin();
    
    if(ams5600.detectMagnet() == 0) {
        Serial.println("Magnet not detected!");
        return;
    }
    
    magnetDetected = true;
    Serial.print("Current Magnitude: ");
    Serial.println(ams5600.getMagnitude());
}

float AngleSensor::getAngle() {
    if (!magnetDetected) return -1.0;
    
    word rawAngle = ams5600.getRawAngle();
    return rawAngle * 0.087890625; // Конвертация в градусы
}

bool AngleSensor::isMagnetDetected() {
    return magnetDetected;
}