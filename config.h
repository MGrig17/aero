#ifndef CONFIG_H
#define CONFIG_H

// Пины для тензодатчиков
const int SENSOR1_DT_PIN = 2;
const int SENSOR1_SCK_PIN = 3;
const int SENSOR2_DT_PIN = 4;
const int SENSOR2_SCK_PIN = 5;

// Пины для степпера
const int STEPPER_STEP_PIN = 6;
const int STEPPER_DIR_PIN = 7;
const int STEPPER_ENDSTOP_PIN = 8;

// Пины для I2C LCD
const int LCD_SDA_PIN = A1;
const int LCD_SCL_PIN = A2;

// Настройки системы
const int MAX_VIRTUAL_POSITION = 360;
const unsigned long MEASUREMENT_INTERVAL = 3000;
const int SERIAL_BAUD_RATE = 115200;
const int LCD_I2C_ADDRESS = 0x27;

// Калибровочные коэффициенты по умолчанию
extern float CALIBRATION_FACTOR_1;
extern float CALIBRATION_FACTOR_2;

#endif