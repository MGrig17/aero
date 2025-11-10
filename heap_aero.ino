#include "config.h" // содержание
#include "WeightSensor.h" // см. файл 2LCmesure.ino
#include "StepperController.h" // Steper_with_limit.ino
#include "AngleSensor.h" // магнитный датчик
#include "DisplayHandler.h" // экран
#include "CommandParser.h" // связатор-объединятор

// Переменные для хранения данных
float weight1 = 0, weight2 = 0;
float angle = 0; // угол
unsigned long lastMeasurementTime = 0;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    
    // Инициализация всех модулей
    displayHandler.begin();
    weightSensor.begin();
    stepperController.begin();
    angleSensor.begin();
    
    Serial.println("=== INTEGRATED SYSTEM READY ===");
    commandParser.printHelp();
    
    displayHandler.displayMessage("System Ready", "Waiting...");
}

void loop() {
    // Обработка команд
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        commandParser.handleCommand(command);
    }
    
    // Измерение весов
    if (commandParser.isMeasuring() && 
        millis() - lastMeasurementTime >= MEASUREMENT_INTERVAL) {
        weightSensor.takeMeasurement();
        lastMeasurementTime = millis();
    }
    
    // Чтение данных для дисплея
    weightSensor.readValues(weight1, weight2);
    angle = angleSensor.getAngle();
    
    // Обновление дисплея
    displayHandler.displayWeights(weight1, weight2);
    if (angle >= 0) {
        displayHandler.displayAngle(angle);
    }
    
    // Управление степпером
    stepperController.run();
    
    delay(100);
}