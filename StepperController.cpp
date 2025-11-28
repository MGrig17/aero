#include "StepperController.h"
#include "AngleSensor.h"

StepperController stepperController;

StepperController::StepperController() : isHomed(false) {
}

void StepperController::begin() {
    pinMode(STEPPER_ENDSTOP_PIN, INPUT_PULLUP);
    pinMode(STEPPER_STEP_PIN, OUTPUT);
    pinMode(STEPPER_DIR_PIN, OUTPUT);
    pinMode(TMC_ENABLE_PIN, OUTPUT);
    
    digitalWrite(TMC_ENABLE_PIN, LOW);  // Включить драйвер
    home();
}

// поиск "нуля"
void StepperController::home() {
    Serial.println("Homing...");
    
    // Движение к концевику
    digitalWrite(STEPPER_DIR_PIN, LOW);
    while(digitalRead(STEPPER_ENDSTOP_PIN) != HIGH) {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(2000);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        delayMicroseconds(2000);
    }

    totalSteps = 0; // СБРОС СЧЕТЧИКА ПРИ homing
    
    // Отъезд от концевика
    digitalWrite(STEPPER_DIR_PIN, HIGH);
    for(int i = 0; i < horisontPos; i++) {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(2000);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        delayMicroseconds(2000);
    }
    
    totalSteps += horisontPos; // УЧЕСТЬ ШАГИ ОТЪЕЗДА
    
    isHomed = true;
    angleSensor.resetAngle();
    Serial.println("Homing complete!");
}

// отход на фиксированное количество шагов
void StepperController::moveSteps(int steps) {
    if (!isHomed) {
        Serial.println("Error: Stepper not homed!");
        return;
    }
    
    totalSteps += steps; // ОБНОВЛЯЕМ СЧЕТЧИК
    
    // Serial.print("Moving ");
    // Serial.print(steps);
    // Serial.println(" steps");
    
    // Установка направления
    digitalWrite(STEPPER_DIR_PIN, steps > 0 ? HIGH : LOW);
    
    // Выполнение шагов (взят код из tmc-driver.ino)
    for (int i = 0; i < abs(steps); i++) {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(2000);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        delayMicroseconds(2000);
    }
    
    //Serial.println("Move complete!");
}

void StepperController::printStatus() {
    Serial.print("Stepper status: Homed, Total steps: ");
    Serial.println(totalSteps);
}

void StepperController::run() {
    // Пустая функция, но должна быть объявлена для совместимости с другими компонентами
}
