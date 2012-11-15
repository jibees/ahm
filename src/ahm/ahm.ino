// http://www.arduino.cc/en/Tutorial/LiquidCrystal
#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"

// initialize the library with the numbers of the interface pins
// TODO LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
String currentDate;   
int zone1_Output = 9;
int zone2_Output = 10;
int zone3_Output = 11;
int zone4_Output = 12;

void setup() {
  // -- LDCD PART
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Starting AHM...);
  
  // -- RTC PART
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    lcd.print("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__));
    return;
  }
  // reserve 200 bytes for the currentDate:
  currentDate.reserve(200);
  
  // -- OUTPUT PART
  pinMode(zone1_Output, OUTPUT);
  pinMode(zone2_Output, OUTPUT);
  pinMode(zone3_Output, OUTPUT);
  pinMode(zone4_Output, OUTPUT);
}

void loop() {
  // Get current date from the RTC
  DateTime now = RTC.now();
  // Print the date on the LCD
  lcd.print(getCurrentDateAsString(now));
  


  // Wait 3s & restart the loop...
  delay(3000);
}

String getCurrentDateAsString(DateTime now) {
  // Re-init...
  currentDate = "";
  currentDate += now.weekday();
  currentDate += " ";
  currentDate += now.day();
  currentDate += " ";
  currentDate += now.month();
  currentDate += " ";
  currentDate += now.hour();
  currentDate += ":";
  currentDate += now.minute();
  
} 
