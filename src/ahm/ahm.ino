#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
//DL here http://code.google.com/p/arduino-pinchangeint/downloads/list
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
//DL here https://github.com/adafruit/RTClib

//RTC
RTC_DS1307 RTC;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(9, 8, 5, 4, 3, 2);

// Timezone shifting : we are in Europe/Paris ie. +1 
const int timezoneShiftInHour = 1;

// Set the output values
const int zone1Output = 11;
const int zone2Output = 12;
const int zone3Output = 13;
const int pushButton1 = 6;

// Define consts about mode eco and mode confort
const int modeEco = HIGH;
const int modeConfort = LOW;

// Create and update the zone value (by default on confort)
int zone1ModeValue = modeConfort;
int zone2ModeValue = modeConfort;
int zone3ModeValue = modeConfort;

//bounce management
#define BOUNCE_DURATION 500
volatile unsigned long bounceTime=0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Booting..."); 
  
  lcd.setCursor(0, 0);
  lcd.print("RTC Init... ");
  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  
  //configure interrupt
  pinMode(pushButton1, INPUT);
  PCintPort::attachInterrupt(pushButton1, button1Pushed , RISING); 
  
  delay(1000);
}

void button1Pushed() {
  if(millis() > bounceTime)  {
      Serial.println("button1Pushed");
      bounceTime = millis() + BOUNCE_DURATION;  // set whatever bounce time in ms is appropriate
  }
}

void loop() {
  // Manage zone by zone
  manageZone1();
  manageZone2();
  manageZone3();
 
  // Update the relays
  updateTheRelays();
  
  // Print the Date on row 1
  lcd.setCursor(0, 0);
  lcd.print(getPrintableDate());
  
  // Print the zones on row 2
  lcd.setCursor(0, 1);
  lcd.print(getPrintableZones());
  
  delay(1000);
}

void updateTheRelays() {
  digitalWrite(zone1Output, zone1ModeValue);
  Serial.println("ZONE 1:" + String(zone1ModeValue));
  
  digitalWrite(zone2Output, zone2ModeValue);
  Serial.println("ZONE 2:" + String(zone2ModeValue));
  
  digitalWrite(zone3Output, zone3ModeValue);
  Serial.println("ZONE 3:" + String(zone3ModeValue));
}

// CHAMBRE DE MATILDA
void manageZone1() {
  DateTime date = RTC.now();
  int realHour = date.hour() + timezoneShiftInHour;
  switch (date.dayOfWeek()) {
    case 1:
    case 7:
      // Weekend, Saturday, Sunday : Always ON on sleep time
      if (realHour < 8) {
        zone1ModeValue = modeConfort;
      } else if (realHour < 10) {
         zone1ModeValue = modeEco;
      } else if (realHour <= 12) {
        zone1ModeValue = modeConfort;
      } else if (realHour <= 15) {
        zone1ModeValue = modeEco;
      } else if (realHour <= 17) {
        zone1ModeValue = modeConfort;
      } 
     
      break;
    default:
      // On week modeConfort : 17h-8h
      if (realHour < 8 || realHour > 17) {
        zone1ModeValue = modeConfort;
      } else {
        // be default, mode eco
        zone1ModeValue = modeEco;
      }
      break;
  } 
}


// CHAMBRE DES PARENTS
void manageZone2() {
  DateTime date = RTC.now();
  // modeConfort 22h-00h
  int realHour = date.hour() + timezoneShiftInHour;
  if (22 < realHour  && realHour <= 23) {
    zone2ModeValue = modeConfort;
  } else {
    // bu default, mode eco
    zone2ModeValue = modeEco;
    Serial.print(zone2ModeValue);
  }
}


// SALON
void manageZone3() {
  DateTime date = RTC.now();
  int realHour = date.hour() + timezoneShiftInHour;
  switch (date.dayOfWeek()) {
    case 1:
    case 7:
      // Saturday, Sunday
      if (realHour < 7 || realHour >= 23) {
        // On night, mode eco
        zone3ModeValue = modeEco;
      } else {
        zone3ModeValue = modeConfort;  
      }
      
      break;
    default:
      // On week
      // modeConfort : 17h-23h && 7h-9h
      if ((17 < realHour && realHour < 23) || (7 < realHour && realHour < 9 )) {
        zone3ModeValue = modeConfort;
      } else {
        zone3ModeValue = modeEco;
      }
      break;
  } 
}



String getPrintableZones() {
  String printableZones = String();
  
  // ZONE 1
  printableZones = "1:";
  printableZones += getModeStringValue(zone1ModeValue);
  // ZONE 2
  printableZones += " ";
  printableZones += "2:";
  printableZones += getModeStringValue(zone2ModeValue);
  // ZONE 3
  printableZones += " ";
  printableZones += "3:";
  printableZones += getModeStringValue(zone3ModeValue);
  return printableZones;
}
 
String getModeStringValue(int zoneModeValue) {
  switch(zoneModeValue) {
   case modeEco:
     return "Ec";
   case modeConfort:
     return "Co";
  }   
}

String getPrintableDate() {
  
  DateTime date = RTC.now();
  
  Serial.println(date.unixtime());
  //Serial.println(now());
  
   // Lu 23 11 16:03:09
  String printableDate = String();
  // Day Of Week
  printableDate = String(getDayOfWeek(date.dayOfWeek())); 
  // Day in Month
  printableDate += " "; 
  printableDate += printDigitForDate(date.day());
  // Month
  printableDate += "/"; 
  printableDate += printDigitForDate(date.month());
  
  // Hour
  printableDate += " ";
  printableDate += printDigitForDate(date.hour()+timezoneShiftInHour);
  // Minute
  printableDate += ":";
  printableDate += printDigitForDate(date.minute());
  
  //Serial.println(printableDate);
  return printableDate;
}

String getDayOfWeek(int weekday){
  switch(weekday + 1) {
    case 1:
      return "Sun";
    case 2:
      return "Mon";
    case 3:
      return "Tue";
    case 4:
      return "Wed";
    case 5:
      return "Thu";
    case 6:
      return "Fri";
    case 7:
      return "Sat";
  }
}

String printDigitForDate(int digits) {
  String digitsString = String();
  if (digits < 10) {
    digitsString = "0";
    digitsString += String(digits);
  } else {
    digitsString = String(digits);
  }
  //Serial.println(digitsString);
  return digitsString;
}
