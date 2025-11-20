  #include "DisplayHandler.h"
//   #include <LiquidCrystal_I2C.h> -- уже есть в "DisplayHandler.h"
// диспетчер команд.

DisplayHandler displayHandler;

DisplayHandler::DisplayHandler() 
    : lcd(LiquidCrystal_I2C(LCD_I2C_ADDRESS, 16, 2)) {  // Прямая инициализация
}

void DisplayHandler::begin() {
    Wire.begin();
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("Angle: "); 
}

void DisplayHandler::displayWeights(float weight1, float weight2) {
    // ВТОРАЯ строка для весов
    lcd.setCursor(0, 1);
    lcd.print("1:");
    lcd.print(weight1, 1);
    lcd.print(" 2:");
    lcd.print(weight2, 1);
    lcd.print("   ");
}

void DisplayHandler::displayAngle(float angle) {
    // Первая строка для угла 
    lcd.setCursor(7, 0);  
    lcd.print(angle, 1);
    lcd.print((char)223); 
    lcd.print("   ");     
}

void DisplayHandler::displayNoMagnet() {
    // Сообщение об отсутствии магнита
    lcd.setCursor(0, 1);
    lcd.print("No Magnet!    ");
}
// очистка дисплея
void DisplayHandler::clear() {
    lcd.clear();
}
