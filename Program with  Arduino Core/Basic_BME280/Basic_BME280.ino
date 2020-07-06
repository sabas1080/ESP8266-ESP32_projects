/*
  Communicate with BME280s with ESP32 I2C
  Based in the work of:
  Nathan Seidle @ SparkFun Electronics

  This example shows how to connect a sensor BME280 with ESP32.

  Hardware connections:
  BME280 -> ESP32
  GND -> GND
  3.3 -> 3.3
  SDA -> GPIO 21
  SCL -> GPIO 22
*/

#include <Wire.h>

#include "SparkFunBME280.h"
BME280 mySensor; //Uses default I2C address 0x77 or address 0x76 (jumper closed)

void setup()
{
  Serial.begin(9600);
  Serial.println("Example showing alternate I2C addresses");

  Wire.begin();

  mySensor.setI2CAddress(0x76); //The default for the SparkFun Environmental Combo board is 0x77 (jumper open).
  //If you close the jumper it is 0x76
  //The I2C address must be set before .begin() otherwise the cal values will fail to load.

  if(mySensor.beginI2C() == false) {
  Serial.println("Sensor connect failed");
  while(1);
  }
}

void loop()
{
  Serial.print("Humidity: ");
  Serial.print(mySensor.readFloatHumidity(), 0);

  Serial.print(" Pressure: ");
  Serial.print(mySensor.readFloatPressure(), 0);

  Serial.print(" Temp: ");
  //Serial.print(mySensor.readTempC(), 2);
  Serial.print(mySensor.readTempC(), 2);

  Serial.println();

  delay(50);
}
