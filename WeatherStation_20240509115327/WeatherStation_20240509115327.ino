// include relevant libraries for the Arduino, sensor, and Real-time clock (RTC)
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>

// Define DHT pin
#define DHTPIN 2        // Pin the sensor is connected to on Arduino (can be any number from 2 - 13)
#define DHTTYPE DHT22   // DHT22 is the type of temperature sensor we are using  

File myFile;

// Initialize DHT sensor for normal 16MHz Arduino
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 RTC;

// Define threshold values for temperature and humidity
const float TEMPERATURE_THRESHOLD = 45.0; // Celsius
const float HUMIDITY_THRESHOLD = 80.0;    // Percentage

void setup() {   // setup function runs once
  Serial.begin(9600); // Opens serial connection, sets the data rate to 9600 bps (bits per second) and waits for the port to open
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("Initialization failed!");
    while (1);
  }
  Serial.println("Initialization done.");
  // Create a new data file if one does not already exist
  if (!SD.exists("DATA.txt")) {
    myFile = SD.open("DATA.txt", FILE_WRITE);
    myFile.print("DATE, TIME, TEMP *C, HUMIDITY");
    myFile.println();
    myFile.close();
  }
  // Initializing the DHT and RTC
  dht.begin();
  Wire.begin();
  RTC.begin();
  // Check to see if the RTC is keeping time
  if (!RTC.begin()) {
    Serial.println("RTC is NOT running!");
    // Sets the RTC to the time that this sketch was compiled
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = RTC.now(); // Query the RTC for the current time
  // Read and store the temperature and humidity
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Check if temperature or humidity exceeds the threshold
  if (t > TEMPERATURE_THRESHOLD || h > HUMIDITY_THRESHOLD) {
    // Alert mechanism - Blink LED, send notification, etc.
    Serial.println("ALERT: Temperature or humidity exceeded threshold!");
    // Add your alert mechanism here
  }

  // Write the current date, time, temperature, and humidity to SD card
  myFile = SD.open("DATA.txt", FILE_WRITE);
  myFile.print(now.day(), DEC);
  myFile.print('/');
  myFile.print(now.month(), DEC);
  myFile.print('/');
  myFile.print(now.year(), DEC);
  myFile.print(',');
  myFile.print(' ');
  myFile.print(now.hour(), DEC);
  myFile.print(':');
  // Inserts a 0 before minutes from 1-9 so that the time displays correctly, i.e., 12:05 instead of 12:5
  if (now.minute() < 10) {
    myFile.print('0');
    myFile.print(now.minute(), DEC);
  }
  else {
    myFile.print(now.minute(), DEC);
  }
  myFile.print(',');
  myFile.print(' ');
  myFile.print(t);
  myFile.print(',');
  myFile.print(' ');
  myFile.print(h);
  myFile.println();
  myFile.close();

  // Write the current date, time, temperature, and humidity to the serial monitor
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(',');
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  if (now.minute() < 10) {
    Serial.print('0');
    Serial.print(now.minute(), DEC);
  }
  else {
    Serial.print(now.minute(), DEC);
  }
  Serial.print(',');
  Serial.print(' ');
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" *C");
  Serial.print(',');
  Serial.print(' ');
  Serial.print("Hum: ");
  Serial.print(h);
  Serial.println();

  delay(10000); // Wait time until the device reads the temperature again
}
