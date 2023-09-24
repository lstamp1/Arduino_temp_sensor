// Liam Stamp - September 2023
// Temperature Sensor Data Recording using Interrupts
// This code prints out a time stamp every second and will also print out temperature in farrenheight if button is pressed
// PIN 2 = Button NC
// PIN 4 = Temp sensor data (YELLOW WIRE)
// Temp Sensor RED=5V, BLACK=GND
// Serial monitor = 9600 New Line



// Include the following:
#include <OneWire.h>
#include <DallasTemperature.h>

//Define pins being used
#define temp_data 4  // temp data plugged into pin 4 (YELLOW WIRE)
#define button_pin 2
#define debounce_delay 200  // 200 milliseconds for debounce

OneWire oneWire(temp_data);
DallasTemperature sensors(&oneWire);

volatile bool readTemperatureFlag = false;
float latestTemperature; // To store the latest temperature reading

unsigned long time_stamp = 0;
unsigned long lastInterruptTime = 0; // for debouncing

void setup() {
  Serial.begin(9600);
  sensors.begin();

  pinMode(button_pin, INPUT);  // Configured as normally LOW and will be HIGH when pressed
  attachInterrupt(digitalPinToInterrupt(button_pin), buttonPressISR, RISING);
}

void loop() {
  if (readTemperatureFlag) {
    //Serial.println();
    Serial.print(time_stamp);
    Serial.print(", ");
    Serial.print(latestTemperature);
    Serial.println(", F");
    readTemperatureFlag = false;
  }
  else {Serial.println(time_stamp);}
  time_stamp++;
  delay(1000);
}

void buttonPressISR() {
  unsigned long interruptTime = millis();

  // Debounce the button
  if (interruptTime - lastInterruptTime > debounce_delay) {
    sensors.requestTemperatures();
    latestTemperature = sensors.getTempFByIndex(0)+0; //added one deg for calibration purposes
    readTemperatureFlag = true;
  }

  lastInterruptTime = interruptTime;
}
