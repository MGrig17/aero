#ifndef STEPPER_CONTROLLER_H
#define STEPPER_CONTROLLER_H

#include "AccelStepper.h"
#include "config.h"

class StepperController {
private:
    AccelStepper stepper;
    int currentRealPosition;
    bool isHomed;
    
public:
    StepperController();
    void begin();
    void home();
    void moveSteps(int steps);
    void moveToVirtualPosition(int virtualPos);
    int getVirtualPosition();
    void printStatus();
    void run();
};

extern StepperController stepperController;

#endif