#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Пины для тензодатчиков
const int SENSOR1_DT_PIN = 5;
const int SENSOR1_SCK_PIN = 6;
const int SENSOR2_DT_PIN = 8;
const int SENSOR2_SCK_PIN = 7;

// Пины для степпера
const int STEPPER_STEP_PIN = 4;
const int STEPPER_DIR_PIN = 3;
const int STEPPER_ENDSTOP_PIN = 13;
const int TMC_ENABLE_PIN = 8;  // Новый пин для включения драйвера

// Настройки системы
//const int MAX_VIRTUAL_POSITION = 360;
const unsigned long MEASUREMENT_INTERVAL = 3000;
const int SERIAL_BAUD_RATE = 9600;
const int LCD_I2C_ADDRESS = 0x3F;

//// Калибровочные коэффициенты по умолчанию
//extern float CALIBRATION_FACTOR_1;
//extern float CALIBRATION_FACTOR_2;

#endif
