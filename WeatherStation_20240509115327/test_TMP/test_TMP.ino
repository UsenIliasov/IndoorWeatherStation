//include relevant libraries for the Arduino, sensor and Real time clock (RTC)
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h" 


//define DHT pin
#define DHTPIN 2        //  pin the sensor is connected to on arduino (can be any number from 2 - 13)
#define DHTTYPE DHT22   // DHT22 is the type of temperature sensor we are using  

File myFile;

// initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 RTC;


void setup () {   // setup function runs once

  
  
  //initializing the DHT and RTC
  dht.begin();
  Wire.begin();
  RTC.begin();

  // Check to see if the RTC is keeping time.  
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Sets the RTC to the time that this sketch was compiled 
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}