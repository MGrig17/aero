#include "AngleSensor.h"

AngleSensor angleSensor;

AngleSensor::AngleSensor() : magnetDetected(false), angleOffset(0) {
} // конструктор со списокм инициализаций

void AngleSensor::begin() {
    // // Добавить таймаут для detectMagnet
    // unsigned long startTime = millis();
    // while(ams5600.detectMagnet() == 0) {
    //     if (millis() - startTime > 3000) { // 3 секунды таймаут
    //         Serial.println("Magnet detection timeout!");
    //         magnetDetected = false;
    //         return;
    //     }
    // }

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