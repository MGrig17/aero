#ifndef STEPPER_CONTROLLER_H
#define STEPPER_CONTROLLER_H

#include <Arduino.h>
//#include "AccelStepper.h"
#include "AngleSensor.h"
#include "config.h"

class StepperController {
private:
    // AccelStepper stepper;
    long horisontPos = 590;
    long totalSteps;  // ← ДОБАВИТЬ СЧЕТЧИК шагов относительно положения home
    bool isHomed;
    
public:
    StepperController();
    void begin();
    void home();
    void moveSteps(int steps);
    void printStatus();
    void run();
    long getTotalSteps() { return totalSteps; }  // ← ГЕТТЕР
};

extern StepperController stepperController;

#endif