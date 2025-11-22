#ifndef ANGLE_SENSOR_H
#define ANGLE_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <AS5600.h>

class AngleSensor {
private:
    AMS_5600 ams5600;
    bool magnetDetected; // определение наличия магнита
    float angleOffset;  // смещение
    float convertRawAngleToDegrees(word newAngle); // заменила float getRawAngle(); 
    
public:
    AngleSensor();
    void begin();
    void printAngle();
    float getAngle();
    bool isMagnetDetected();
    void resetAngle();
};

extern AngleSensor angleSensor;

#endif
