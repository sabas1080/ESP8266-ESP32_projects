/*
  ArduinoMqttClient - WiFi MQTT with NodeRed

  This example connects to a MQTT broker and subscribes to a single topic,
  it also publishes a message to the same topic once a second.
  When a message is received it prints the message to the serial monitor.

  The circuit:
  - Arduino MKR 1000, MKR 1010 or Uno WiFi Rev.2 board and ESP32

  This example code is in the public domain.
*/

#include <ArduinoMqttClient.h>
#include <WiFi.h> // for MKR1000 change to: #include <WiFi101.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int port = 1883;

const char topicLamp[]  = "sensores/lamp";
const char topicTemp[]  = "sensores/temp";

const long interval = 1000;
unsigned long previousMillis = 0;

String statusLed;

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  Serial.print("Subscribing to topic: ");
  Serial.println(topicLamp);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topicLamp);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topicLamp);
  Serial.println();

  pinMode(2,OUTPUT);
}

void loop() {
  // check for incoming messages
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // use the Stream interface to print the contents
    while (mqttClient.available() > 0) {
      statusLed = mqttClient.readString();
    }

    if(statusLed == "on"){
      digitalWrite(2,HIGH);
      Serial.println(statusLed);
    }
    else{
      digitalWrite(2,LOW);
      Serial.println(statusLed);
    }
    
    Serial.println();

    Serial.println();
  }

  // avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    Serial.print("Sending message to topic: ");
    Serial.println(topicTemp);
    Serial.print((temprature_sens_read() - 32) / 1.8);
    Serial.println("ºC");

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topicTemp);
    // Convert raw temperature in F to Celsius degrees
    mqttClient.print((temprature_sens_read() - 32) / 1.8);
    mqttClient.endMessage();

    Serial.println();
  }
}
