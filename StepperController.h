#ifndef STEPPER_CONTROLLER_H
#define STEPPER_CONTROLLER_H

#include <Arduino.h>
#include "AccelStepper.h"
#include "config.h"

class StepperController {
private:
    AccelStepper stepper;
    bool isHomed;
    
public:
    StepperController();
    void begin();
    void home();
    void moveSteps(int steps);
    void printStatus();
    void run();
};

extern StepperController stepperController;

#endif