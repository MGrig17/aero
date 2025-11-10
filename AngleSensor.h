#ifndef ANGLE_SENSOR_H
#define ANGLE_SENSOR_H

#include <Wire.h>
#include <AS5600.h>

class AngleSensor {
private:
    AMS_5600 ams5600;
    bool magnetDetected;
    
public:
    AngleSensor();
    void begin();
    float getAngle();
    bool isMagnetDetected();
};

extern AngleSensor angleSensor;

#endif