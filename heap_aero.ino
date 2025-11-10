#include "config.h"
#include "WeightSensor.h"
#include "StepperController.h"
#include "AngleSensor.h"
#include "DisplayHandler.h"
#include "CommandParser.h"

// Переменные для хранения данных
float weight1 = 0, weight2 = 0;
float angle = 0;
unsigned long lastMeasurementTime = 0;
unsigned long lastDisplayUpdate = 0;  // ← ДОБАВИТЬ

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    
    // Инициализация всех модулей
    displayHandler.begin();
    weightSensor.begin();
    stepperController.begin();
    angleSensor.begin();
    
    Serial.println("=== INTEGRATED SYSTEM READY ===");
    commandParser.printHelp();
}

void loop() {
    // Обработка команд
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        commandParser.handleCommand(command);
    }
    
    // Измерение весов для Serial
    if (commandParser.isMeasuring() && 
        millis() - lastMeasurementTime >= MEASUREMENT_INTERVAL) {
        weightSensor.takeMeasurement();
        lastMeasurementTime = millis();
    }
    
    // Обновление дисплея каждые 500мс (не каждый цикл!)
    if (millis() - lastDisplayUpdate >= 500) {
        // Чтение данных
        weightSensor.readValues(weight1, weight2);
        angle = angleSensor.getAngle();
        
        // Обновляем ВЕСА (первая строка)
        displayHandler.displayWeights(weight1, weight2);
        
        // Обновляем УГОЛ или сообщение (вторая строка)
        if (angle >= 0) {
            displayHandler.displayAngle(angle);
        } else {
            displayHandler.displayNoMagnet();
        }
        
        lastDisplayUpdate = millis();
    }
    
    // Управление степпером
    stepperController.run();
    
    delay(100);
}