#include <DS1307RTC.h>
#include <Time.h>  
#include <Wire.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(9, 8, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("[Booting...]"); 
  
  //time
  Serial.print("RTC Init");
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet) {
    Serial.println("\t\t\t[NOK]");
    Serial.println("Unable to sync with the RTC");
  } else {
    Serial.println("\t\t\t[OK]");   
  }
  //RTC.set(1353506130);   // set the RTC and the system time to the received value
  //setTime(1353506130);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.print("hello, world!");
  
}

void loop() {
   // digitalClockDisplay();
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print(hour());
  lcd.setCursor(2, 1);
  lcd.print(":");
  lcd.setCursor(3, 1);
  lcd.print(minute());
  lcd.print(":");
  lcd.setCursor(6, 1);
  lcd.print(second());
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
