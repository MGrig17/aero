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
    static unsigned long lastDisplayUpdate = 0;
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
        
        // ВЫВОД В SERIAL (как в SerialDisplay.ino)
        if (angle >= 0) {
            Serial.println(angle);
        }
        
        lastDisplayUpdate = millis();
    }
    
    stepperController.run();
    delay(50);
}
