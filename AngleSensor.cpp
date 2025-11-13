#include "AngleSensor.h"

AngleSensor angleSensor;

AngleSensor::AngleSensor() : magnetDetected(false), angleOffset(0) {
}

void AngleSensor::begin() {
    Wire.begin();
    
    if(ams5600.detectMagnet() == 0) {
        Serial.println("Magnet not detected!");
        magnetDetected = false;
        return;
    }
    
    magnetDetected = true;
    
    // КАЛИБРОВКА НУЛЯ (из SerialDisplay.ino)
    angleOffset = getRawAngle();
    Serial.print("Angle calibrated. Offset: ");
    Serial.println(angleOffset);
}

// Конвертация в градусы
float AngleSensor::getRawAngle() {
    return ams5600.getRawAngle() * 0.087890625; 
}

float AngleSensor::getAngle() {
    if (!magnetDetected) return -1.0;
    
    // КАЛИБРОВАННЫЙ УГОЛ (из SerialDisplay.ino)
    float angle = getRawAngle() - angleOffset;
    if (angle < 0) angle = -angle; // Абсолютное значение
    
    return angle;
}
