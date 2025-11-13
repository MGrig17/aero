#ifndef ANGLE_SENSOR_H
#define ANGLE_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <AS5600.h>

class AngleSensor {
private:
    AMS_5600 ams5600;
    bool magnetDetected;
    float angleOffset;  // смещение
    
public:
    AngleSensor();
    void begin();
    float getAngle();
    bool isMagnetDetected();
    float getCalibratedAngle();  // ← ДОБАВИТЬ калиброванный угол
};

extern AngleSensor angleSensor;

#endif
