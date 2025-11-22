#include "AngleSensor.h"

AngleSensor angleSensor;

AngleSensor::AngleSensor() : magnetDetected(false), angleOffset(0) {
} // конструктор со списокм инициализаций

void AngleSensor::begin() {

    if(ams5600.detectMagnet() == 0) {
        Serial.println("Magnet not detected!"); // в этом if проверяем наличие магнита, если его нет -- выбрасываем рпедупреждение, если нет -- скипаем
        magnetDetected = false;
        return;
    }
    
    magnetDetected = true; // есть магнит    
  // Устанавливаем текущее положение как 0
    angleOffset = convertRawAngleToDegrees(ams5600.getRawAngle()); // 
    Serial.print("Angle calibrated. Offset: ");
    Serial.println(angleOffset);
}

void AngleSensor::printAngle() {
    if (!magnetDetected) {
        Serial.println("No magnet detected!");
        return;
    }
    
    float angle = getAngle();
    Serial.print("Current angle: ");
    Serial.print(angle, 1);
    Serial.println("°");
}

float AngleSensor::convertRawAngleToDegrees(word newAngle) {  // ← Добавить AngleSensor::
    return newAngle * 0.087890625;
}

// Получение калиброванного угла
float AngleSensor::getAngle() {          
    word rawAngle = ams5600.getRawAngle();
    float angle = convertRawAngleToDegrees(rawAngle) - angleOffset;
    if (angle < 0) angle += 360;
    return angle;
}

void AngleSensor::resetAngle() {
    if (!magnetDetected) {
        Serial.println("Cannot reset angle - no magnet!");
        return;
    }
    
    angleOffset = convertRawAngleToDegrees(ams5600.getRawAngle());
    Serial.print("Angle reset to zero. Offset: ");
    Serial.println(angleOffset);
}