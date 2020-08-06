/************************************************************
MQTT Calback and BMP280.ino

Andres Sabas @ Electronic Cats
Original Creation Date: Agu 6, 2020

This example demonstrates how to use the MQTT client
functionality of the callback

Development environment specifics:
  IDE: Arduino 1.8.9
  Hardware Platform:
  ESP8266 and ESP32

This code is beerware; if you see me (or any other Electronic Cats
member) at the local, and you've found our code helpful,
please buy us a round!

Distributed as-is; no warranty is given.
************************************************************/
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "SparkFunBME280.h"
#include <ArduinoJson.h>

// Allocate the JSON document
//
// Inside the brackets, 200 is the RAM allocated to this document.
// Don't forget to change this value to match your requirement.
// Use arduinojson.org/v6/assistant to compute the capacity.
//StaticJsonDocument<200> doc;
const size_t capacity = JSON_OBJECT_SIZE(4);
DynamicJsonDocument doc(capacity);

BME280 mySensor; //Uses default I2C address 0x77 or address 0x76 (jumper closed)

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)


IPAddress mqtt_server(162, 243, 173, 161); //http://162.243.173.161/
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("test", "hello world");
      // ... and resubscribe
      client.subscribe("control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 10 seconds before retrying
      delay(10000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Start a new line
  Serial.println();

  Wire.begin();

  mySensor.setI2CAddress(0x76); //The default for the SparkFun Environmental Combo board is 0x77 (jumper open).
  //If you close the jumper it is 0x76
  //The I2C address must be set before .begin() otherwise the cal values will fail to load.

  if (mySensor.beginI2C() == false) {
    Serial.println("Sensor connect failed");
    while (1);
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop()
{
  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Add values in the document
    doc["sensor"] = "bme280";
    doc["humd"] = mySensor.readFloatHumidity();
    doc["press"] = mySensor.readFloatPressure();
    doc["temp"] = mySensor.readTempC();

    // Generate the minified JSON and send it to the Serial port.
    serializeJson(doc, Serial);
    Serial.println();

    char buffer[256];
    size_t n = serializeJson(doc, buffer);
    Serial.println("Publish message: ");
    client.publish("bme280", buffer, n);
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
