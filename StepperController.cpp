#include "StepperController.h"

StepperController stepperController;

StepperController::StepperController() 
    : stepper(AccelStepper(1, STEPPER_STEP_PIN, STEPPER_DIR_PIN)),
      isHomed(false) {
}

void StepperController::begin() {
    pinMode(STEPPER_ENDSTOP_PIN, INPUT_PULLUP);
    stepper.setMaxSpeed(2000);
    stepper.setAcceleration(1000);
    home();
}

void StepperController::home() {
    Serial.println("Homing...");
    
    // Движение к концевику (как раньше)
    stepper.setSpeed(200);
    while(digitalRead(STEPPER_ENDSTOP_PIN) != HIGH) {
        stepper.runSpeed();
    }
    
    stepper.stop();
    delay(100);
    stepper.setCurrentPosition(0);
    isHomed = true;
    
    // Отъезд от концевика
    stepper.moveTo(-100);
    while(stepper.distanceToGo() != 0) {
        stepper.run();
    }
    
    Serial.println("Homing complete!");
}

void StepperController::moveSteps(int steps) {
    if (!isHomed) {
        Serial.println("Error: Stepper not homed!");
        return;
    }
    
    // СНИМАЕМ ФИКСАЦИЮ - включаем управление
    stepper.setSpeed(0);  // Сброс скорости
    stepper.enableOutputs(); // Включить выходы драйвера
    
    Serial.print("Moving ");
    Serial.print(steps);
    Serial.println(" steps");
    
    stepper.setSpeed(steps > 0 ? 500 : -500);
    int stepsToGo = abs(steps);
    
    while(stepsToGo > 0) {
        stepper.runSpeed();
        stepsToGo--;
        delay(1);
    }
    stepper.stop();
    
    // ОПЯТЬ ФИКСИРУЕМ после движения
    stepper.setCurrentPosition(0); // Сброс позиции если нужно
    
    Serial.println("Move complete!");
}

void StepperController::printStatus() {
    Serial.print("Current position: ");
    Serial.println(stepper.currentPosition());
}

void StepperController::run() {
    // Пустая - используем только прямое управление из moveSteps
}