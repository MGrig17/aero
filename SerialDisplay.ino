#include <Wire.h>
#include <AS5600.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
AMS_5600 ams5600;

float angleOffset = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  
  lcd.init();
  lcd.backlight();
  lcd.print("Angle: ");
  
  // Проверка магнита
  if(ams5600.detectMagnet() == 0){
    lcd.clear();
    lcd.print("No Magnet!");
    while(1) delay(1000);
  }
  
  // Устанавливаем текущее положение как 0
  angleOffset = convertRawAngleToDegrees(ams5600.getRawAngle());
}

float convertRawAngleToDegrees(word newAngle)
{
  return newAngle * 0.087890625;
}

float getCalibratedAngle()
{
  float angle = convertRawAngleToDegrees(ams5600.getRawAngle()) - angleOffset;
  if (angle < 0) angle *= -1;
  return angle;
}

void loop()
{
  float angle = getCalibratedAngle();
  
  // Вывод на дисплей
  lcd.setCursor(7, 0);
  lcd.print(angle, 1);
  lcd.print((char)223);
  lcd.print("   ");
  
  // Вывод в Serial
  Serial.println(angle);
  
  delay(100);
}