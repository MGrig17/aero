#include "DisplayHandler.h"

DisplayHandler displayHandler;

DisplayHandler::DisplayHandler() 
    : softI2C(SoftWire(LCD_SDA_PIN, LCD_SCL_PIN)),
      lcd(LiquidCrystal_I2C(LCD_I2C_ADDRESS, 16, 2)) {
}

void DisplayHandler::begin() {
    softI2C.begin();
    softI2C.setTimeout(1000);
    
    lcd.init();
    lcd.backlight();
    displayMessage("Initializing...");
    delay(2000);
    displayMessage("System Ready");
    delay(1000);
}

void DisplayHandler::displayWeights(float weight1, float weight2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("1:");
    lcd.print(weight1, 1);
    lcd.print("g");
    
    lcd.setCursor(8, 0);
    lcd.print("2:");
    lcd.print(weight2, 1);
    lcd.print("g");
}

void DisplayHandler::displayAngle(float angle) {
    lcd.setCursor(0, 1);
    lcd.print("Angle:");
    lcd.print(angle, 1);
    lcd.print((char)223); // Символ градуса
}

void DisplayHandler::displayMessage(const String& line1, const String& line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    
    if (line2.length() > 0) {
        lcd.setCursor(0, 1);
        lcd.print(line2);
    }
}

void DisplayHandler::clear() {
    lcd.clear();
}