#include <SoftWire.h>
#include <LiquidCrystal_I2C.h>

// Настройка пинов
#define SDA_PIN A1
#define SCL_PIN A2

SoftWire softI2C(SDA_PIN, SCL_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  Serial.begin(9600);
  
  // Инициализация SoftWire
  softI2C.begin();
  softI2C.setClock(100000);  // 100kHz
  softI2C.setTimeout(1000);
  
  Serial.println("Testing LCD with SoftWire...");
  Serial.print("SDA: A1, SCL: A2, Address: 0x27");
  
  // Проверяем подключение LCD
  softI2C.beginTransmission(0x27);
  byte error = softI2C.endTransmission();
  
  if (error == 0) {
    Serial.println(" - LCD FOUND!");
    
    // Инициализация LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    
    // Выводим тестовые сообщения
    lcd.setCursor(0, 0);
    lcd.print("SoftWire LCD Test");
    lcd.setCursor(0, 1);
    lcd.print("SDA:A1 SCL:A2");
    lcd.setCursor(0, 2);
    lcd.print("Address: 0x27");
    lcd.setCursor(0, 3);
    lcd.print("Software I2C OK!");
    
  } else {
    Serial.println(" - LCD NOT FOUND!");
    Serial.println("Check wiring and address");
  }
}

void loop()
{
  // Мигаем текстом для демонстрации работы
  static unsigned long lastToggle = 0;
  static bool backlightState = true;
  
  if (millis() - lastToggle > 1000) {
    if (backlightState) {
      lcd.noBacklight();
    } else {
      lcd.backlight();
    }
    backlightState = !backlightState;
    lastToggle = millis();
  }
}
