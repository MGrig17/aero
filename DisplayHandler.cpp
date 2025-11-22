  #include "DisplayHandler.h"
//   #include <LiquidCrystal_I2C.h> -- уже есть в "DisplayHandler.h"
// диспетчер команд.

DisplayHandler displayHandler;

DisplayHandler::DisplayHandler() 
//     : lcd(LiquidCrystal_I2C(LCD_I2C_ADDRESS, 16, 2)) {  // Прямая инициализация
// }
    : lcdWeights(LiquidCrystal_I2C(LCD_I2C_ADDRESS_1, 16, 2)),
      lcdAngle(LiquidCrystal_I2C(LCD_I2C_ADDRESS_2, 16, 2)) {
}

void DisplayHandler::begin() {
    Wire.begin();

    // lcd.init();
    // lcd.backlight();
    // lcd.clear();
    // lcd.print("Angle: "); 

    // Инициализация дисплея для весов
    lcdWeights.init();
    lcdWeights.backlight();
    lcdWeights.clear();
    lcdWeights.setCursor(0, 0);
    lcdWeights.print("1TD: 0.0g");
    lcdWeights.setCursor(0, 1);
    lcdWeights.print("2TD: 0.0g");
    
    // Инициализация дисплея для угла
    lcdAngle.init();
    lcdAngle.backlight();
    lcdAngle.clear();
    lcdAngle.setCursor(0, 0);
    lcdAngle.print("Angle: ");
}

void DisplayHandler::displayWeights(float weight1, float weight2) {
    // // ВТОРАЯ строка для весов
    // lcd.setCursor(0, 1);
    // lcd.print("1:");
    // lcd.print(weight1, 1);
    // lcd.print(" 2:");
    // lcd.print(weight2, 1);
    // lcd.print("   ");

    // ТОЛЬКО на дисплей весов (0x3F)
    lcdWeights.setCursor(5, 0);  // После "1TD: "
    lcdWeights.print(weight1, 1);
    lcdWeights.print("g   ");
    
    lcdWeights.setCursor(5, 1);  // После "2TD: "
    lcdWeights.print(weight2, 1);
    lcdWeights.print("g   ");
}

void DisplayHandler::displayAngle(float angle) {

    // ТОЛЬКО на дисплей угла (0x27)
    lcdAngle.setCursor(7, 0);  // После "Angle: "
    lcdAngle.print(angle, 1);
    lcdAngle.print((char)223); 
    lcdAngle.print("   ");     
}

void DisplayHandler::displayNoMagnet() {
    // Сообщение об отсутствии магнита
    lcdAngle.setCursor(7, 0);
    lcdAngle.print("No Magnet!    ");
}
// очистка дисплея ??? не нужна?
void DisplayHandler::clear() {
    lcdWeights.clear();
    lcdAngle.clear();
}
