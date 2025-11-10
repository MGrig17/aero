#include "StepperController.h"

StepperController stepperController;

StepperController::StepperController() 
    : stepper(AccelStepper(1, STEPPER_STEP_PIN, STEPPER_DIR_PIN)),
      currentRealPosition(0), isHomed(false) {
}

void StepperController::begin() {
    pinMode(STEPPER_ENDSTOP_PIN, INPUT_PULLUP);
    stepper.setMaxSpeed(2000);
    stepper.setAcceleration(1000);
    home();
}

void StepperController::home() {
    Serial.println("Homing - moving to end stop...");
    stepper.setSpeed(200);
    while(digitalRead(STEPPER_ENDSTOP_PIN) != HIGH) {
        stepper.runSpeed();
    }
    
    stepper.stop();
    delay(100);
    stepper.setCurrentPosition(0);
    currentRealPosition = 0;
    isHomed = true;
    
    // Отъезд от концевика
    stepper.moveTo(-5);
    while(stepper.distanceToGo() != 0) {
        stepper.run();
    }
    currentRealPosition = stepper.currentPosition();
    
    Serial.println("Homing complete!");
    printStatus();
}

void StepperController::moveSteps(int steps) {
    if (!isHomed) {
        Serial.println("Error: Stepper not homed!");
        return;
    }
    
    int currentVirtual = getVirtualPosition();
    int targetVirtual = currentVirtual + steps;
    
    if (targetVirtual < 0) targetVirtual = 0;
    if (targetVirtual > MAX_VIRTUAL_POSITION) targetVirtual = MAX_VIRTUAL_POSITION;
    
    int targetReal = -targetVirtual;
    
    Serial.print("Moving ");
    Serial.print(steps);
    Serial.print(" steps from virtual ");
    Serial.print(currentVirtual);
    Serial.print(" to ");
    Serial.println(targetVirtual);
    
    stepper.moveTo(targetReal);
}

void StepperController::moveToVirtualPosition(int virtualPos) {
    if (virtualPos < 0) virtualPos = 0;
    if (virtualPos > MAX_VIRTUAL_POSITION) virtualPos = MAX_VIRTUAL_POSITION;
    
    int steps = virtualPos - getVirtualPosition();
    moveSteps(steps);
}

int StepperController::getVirtualPosition() {
    return -currentRealPosition;
}

void StepperController::printStatus() {
    int virtualPos = getVirtualPosition();
    Serial.print("=== Virtual position: ");
    Serial.print(virtualPos);
    Serial.print("/");
    Serial.print(MAX_VIRTUAL_POSITION);
    Serial.print(" | Available: ");
    Serial.print(MAX_VIRTUAL_POSITION - virtualPos);
    Serial.print(" steps BACKWARD, ");
    Serial.print(virtualPos);
    Serial.println(" steps FORWARD ===");
    
    float degrees = (virtualPos * 360.0) / 200.0;
    Serial.print("Approximate angle: ");
    Serial.print(degrees, 1);
    Serial.println("°");
}

void StepperController::run() {
    if (stepper.distanceToGo() != 0) {
        stepper.run();
        currentRealPosition = stepper.currentPosition();
    }
}