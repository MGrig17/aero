#include "CommandParser.h"
#include "WeightSensor.h"
#include "StepperController.h"
#include "AngleSensor.h"
#include "Potentiometer.h"

CommandParser commandParser; // Создание глобального экземпляра парсера команд

CommandParser::CommandParser() // КОНСТРУКТОР
    : continuousMeasure(false), potMode(false), autoMeasure(false),
      startAngle(0), endAngle(0), angleStep(0), currentTargetAngle(0),
      lastStepTime(0) {
}

// Функция перемещения 
void CommandParser::moveToAngle(float targetAngle) {
    float currentAngle = angleSensor.getAngle();

    // Нормализация целевого угла к диапазону 0-360
    while (targetAngle < 0) targetAngle += 360;
    while (targetAngle >= 360) targetAngle -= 360;
    
    // Вычисление минимальной разницы углов
    float angleDiff = targetAngle - currentAngle; 
    
    // Корректировка для минимального пути
    if (angleDiff > 180) angleDiff -= 360;
    else if (angleDiff < -180) angleDiff += 360;
    
    int steps = angleDiff * 16; // 1/0.0625 = 16 шагов ////////////////////////////////////////////// поправить расчет угла 1
    
    if (steps != 0) {
        // Serial.print("Moving to ");
        // Serial.print(targetAngle);
        // Serial.print("° (");
        // Serial.print(steps);
        // Serial.println(" steps)");
        
        stepperController.moveSteps(steps);
        delay(500); // Даем время на стабилизацию
    }
}

void CommandParser::handleCommand(const String& command, Potentiometer& pot) {
    String cmd = command;
    cmd.trim();
    
    
    if (cmd == "stop") {
        continuousMeasure = false;
        Serial.println("=== MEASUREMENTS STOPPED ===");
    } 
    else if (cmd == "tare") {
        weightSensor.tare();
        Serial.println("Tare reset!");
    } 
    else if (cmd == "status") {
        stepperController.printStatus();
        float currentAngle = angleSensor.getAngle();
        Serial.print("Current angle: ");
        Serial.print(currentAngle, 1);
        Serial.println("°");
    } 

    else if (cmd == "home") {
        stepperController.home();
    }

    else if (cmd == "steps") {  
        Serial.print("Total steps from home: ");
        Serial.println(stepperController.getTotalSteps());
    }

    else if (cmd == "start-measure") { 
        // Получаем данные
        float angle = angleSensor.getAngle();
        float weight1, weight2;
        weightSensor.readValues(weight1, weight2);

        continuousMeasure = true;
        potMode = false;
        autoMeasure = false; // ←  выключаем авто-режим
        
        Serial.println("=== CONTINUOUS MEASUREMENTS STARTED ===");
        // Вывод в формате таблицы
        Serial.println("Angle\t\t1TD\t\t2TD");
        Serial.print(angle, 1);
        Serial.print("°\t\t");
        Serial.print(weight1, 2);
        Serial.print("g\t\t");
        Serial.print(weight2, 2);
        Serial.println("g");
    }

    // ↓↓↓ ДОБАВИТЬ НОВУЮ КОМАНДУ measure С ПАРАМЕТРАМИ ↓↓↓
    else if (cmd.startsWith("measure ")) {
        // Парсим параметры: "measure startAngle endAngle step"
        int firstSpace = cmd.indexOf(' ');
        int secondSpace = cmd.indexOf(' ', firstSpace + 1);
        int thirdSpace = cmd.indexOf(' ', secondSpace + 1);
        
        if (firstSpace == -1 || secondSpace == -1) {
            Serial.println("Error: Usage: measure startAngle endAngle step");
            return;
        }

        startAngle = cmd.substring(firstSpace + 1, secondSpace).toFloat();
        endAngle = cmd.substring(secondSpace + 1, thirdSpace).toFloat();
        
        if (thirdSpace != -1) {
            angleStep = cmd.substring(thirdSpace + 1).toFloat();
        } else {
            angleStep = 1.0; // шаг по умолчанию
        }
        
        // Валидация параметров
        if (angleStep == 0) {
            Serial.println("Error: Step cannot be zero!");
            return;
        }
        
        // Настраиваем автоматические измерения
        autoMeasure = true;
        continuousMeasure = false; // Выключаем непрерывный режим
        potMode = false;
        
        currentTargetAngle = startAngle;
        lastStepTime = millis();
        
        Serial.println("=== AUTO MEASUREMENT STARTED ===");
        Serial.print("Range: ");
        Serial.print(startAngle);
        Serial.print("° to ");
        Serial.print(endAngle);
        Serial.print("°, Step: ");
        Serial.print(angleStep);
        Serial.println("°");
        Serial.println("Time\tAngle\t\t1TD\t\t2TD");
        
        // Двигаем к начальному углу
        moveToAngle(startAngle);
    }

    else if (cmd == "mode_auto") {
        potMode = false;  //  РЕЖИМ КОМАНД
        Serial.println("AUTO MODE: Use commands for control");
    }

    else if (cmd == "mode_pot") {
        potMode = true;   // ← РЕЖИМ ПОТЕНЦИОМЕТРА
        continuousMeasure = false;
        Serial.println("POTENTIOMETER MODE: Use pot for control");

        //  Serial.println("Moving -590 steps to initial position...");
        //  stepperController.moveSteps(-590);

        //  pot.setPosition(-590);
    }

    else if (cmd == "pot") {
        Serial.print("Pot position: ");
        Serial.println(pot.getPosition());
    }
    else if (cmd == "pot_reset") {
        pot.setPosition(0);
        Serial.println("Pot position reset to 0");
    }

    else if (cmd == "angle") {
        float currentAngle = angleSensor.getAngle();
        Serial.print("Current angle: ");
        Serial.print(currentAngle, 1);
        Serial.println("°");
    }

    else if (cmd == "help") {
        printHelp();
    } 
    else if (cmd.toInt() != 0 || cmd.startsWith("-")) {
        stepperController.moveSteps(cmd.toInt());
    } 
    else {
        Serial.println("Unknown command: " + cmd);
        printHelp();
    }
}

void CommandParser::printHelp() {
    Serial.println();
    Serial.println(F("=== HELP ==="));
    Serial.println(F("mode_auto   - command mode (default)"));
    Serial.println(F("mode_pot    - potentiometer mode"));
        Serial.println(F("measure (start end step) - auto measurements (degrees)"));
    Serial.println(F("stop        - stop measurements"));  // в mode_auto
    Serial.println(F("tare        - reset tare for sensors"));  // в mode_auto
    Serial.println(F("status      - show stepper status"));  // в mode_auto
    Serial.println(F("steps       - show total steps from home"));  // в mode_auto
    Serial.println(F("home        - home stepper"));  // в mode_auto
    Serial.println(F("N           - move N steps"));  // в mode_auto
    //Serial.println(F("start-measure - read angle + weights")); // в mode_auto
    //Serial.println(F("angle        - return angle"));
    Serial.println(F("help        - show this help"));
    Serial.println(F("=================="));
}
