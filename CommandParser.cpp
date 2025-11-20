#include "CommandParser.h"
#include "WeightSensor.h"
#include "StepperController.h"
#include "AngleSensor.h"

CommandParser commandParser; // Создание глобального экземпляра парсера команд

CommandParser::CommandParser() : measuring(false) {
} // Конструктор класса - инициализирует флаг измерения в false

void CommandParser::handleCommand(const String& command) {
    String cmd = command;
    cmd.trim();
    
    if (cmd == "start") {
        measuring = true;
        Serial.println("=== MEASUREMENTS STARTED ===");
    } 
    else if (cmd == "stop") {
        measuring = false;
        Serial.println("=== MEASUREMENTS STOPPED ===");
    } 
    else if (cmd == "tare") {
        weightSensor.tare();
        Serial.println("Tare reset!");
    } 
//    else if (cmd.startsWith("factor1 ")) {
//        float factor = cmd.substring(8).toFloat();
//        weightSensor.setCalibrationFactor(1, factor);
//        Serial.print("Factor1 set: ");
//        Serial.println(factor, 6);
//    } 
//    else if (cmd.startsWith("factor2 ")) {
//        float factor = cmd.substring(8).toFloat();
//        weightSensor.setCalibrationFactor(2, factor);
//        Serial.print("Factor2 set: ");
//        Serial.println(factor, 6);
//    } 
    else if (cmd == "status") {
        stepperController.printStatus();
    } 
    else if (cmd == "home") {
        stepperController.home();
    }
    else if (cmd == "angle") {  
        angleSensor.printAngle();
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
    Serial.println("=== HELP ===");
    Serial.println("start       - start weight measurements");
    Serial.println("stop        - stop measurements");
    Serial.println("tare        - reset tare for sensors");
    // Serial.println("factor1 X   - set calibration factor 1");
    // Serial.println("factor2 X   - set calibration factor 2");
    Serial.println("angle       - read current angle");
    Serial.println("status      - show stepper status");
    Serial.println("home        - home stepper");
    Serial.println("N           - move N steps (positive/negative)");
    Serial.println("help        - show this help");
    Serial.println("==================");
}
