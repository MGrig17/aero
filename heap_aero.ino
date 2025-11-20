/*
Ахтунг! 
Перед запуском:
    1. Проверь схему, все ли туда воткнуто.
    2. Перепиши пины в config.h


*/


#include "config.h"
#include "WeightSensor.h"
#include "StepperController.h"
#include "AngleSensor.h"
#include "DisplayHandler.h"
#include "CommandParser.h"

float weight1 = 0, weight2 = 0;
float angle = 0;
unsigned long lastMeasurementTime = 0;
unsigned long lastDisplayUpdate = 0;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    delay(2000);  // Важно: дать время на открытие Serial Monitor
    
// Тк шляпа с датчиком угла -- тестим.

//========DEBUG START=========

 // Сначала I2C сканер
    Serial.println("=== I2C SCANNER ===");
    Wire.begin();
    byte error, address;
    int nDevices = 0;
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.print("I2C device found at 0x");
            Serial.println(address, HEX);
            nDevices++;
        }
    }
    if (nDevices == 0) {
        Serial.println("No I2C devices found");
    }
    Serial.println("=== SCAN COMPLETE ===");

//========DEBUG END=========

    // Инициализация всех модулей
    displayHandler.begin();
    weightSensor.begin();
    stepperController.begin();

    Serial.println("Before angle sensor...");
    angleSensor.begin(); // ← Если зависнет здесь, будем знать
    Serial.println("After angle sensor...");
    
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
    
    // ОБНОВЛЕНИЕ ДИСПЛЕЯ (как в SerialDisplay.ino)
    //static unsigned long lastDisplayUpdate = 0;
    if (millis() - lastDisplayUpdate >= 100) {
        float angle = angleSensor.getAngle();
        
        // ВЫВОД УГЛА (первая строка) - как в SerialDisplay.ino
        if (angle >= 0) {
            displayHandler.displayAngle(angle);
        } else {
            displayHandler.displayNoMagnet();
        }
        
        // ВЫВОД ВЕСОВ (вторая строка)
        weightSensor.readValues(weight1, weight2);
        displayHandler.displayWeights(weight1, weight2);
        
        // // НЕПРЕРЫВНЫЙ ВЫВОД ПОКАЗАНИЙ МАГНИТНОГО ДАТЧИКА В SERIAL (как в SerialDisplay.ino) -- по сути для отладки
        // if (angle >= 0) {
        //     Serial.println(angle);
        // }
        
        lastDisplayUpdate = millis();
    }
    
    stepperController.run();
    delay(50);
}
