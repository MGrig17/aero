#include <SoftWire.h>
#include <LiquidCrystal_I2C.h>

// Создаем SoftWire объект с пинами A1 (SDA) и A2 (SCL)
SoftWire softI2C(A1, A2);

// Инициализируем LCD с SoftWire и адресом 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  // Инициализируем SoftWire
  softI2C.begin();
  softI2C.setTimeout(1000);
  
  // Инициализируем LCD с SoftWire
  lcd.init(softI2C);                    // initialize the lcd with SoftWire
  lcd.backlight();
  
  // Print a message to the LCD.
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Ywrobot Arduino!");
  lcd.setCursor(0,2);
  lcd.print("Arduino LCM IIC 2004");
  lcd.setCursor(2,3);
  lcd.print("Power By Ec-yuan!");
}

void loop()
{
}