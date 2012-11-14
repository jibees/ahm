// http://www.arduino.cc/en/Tutorial/LiquidCrystal
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins

// TODO LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Starting AHM...);
}

void loop() {

}
