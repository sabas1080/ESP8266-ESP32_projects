/*
  ArduinoMqttClient - WiFi Simple Receive for turn on a led

  This example connects to a MQTT broker and subscribes to a single topic.
  When a message is received it prints the message to the serial monitor 
  and turn on a led.

  The circuit:
  - ESP32, Arduino MKR 1000, MKR 1010 or Uno WiFi Rev.2 board

  This example code is in the public domain.
  Este código es beerware si tu me ves ( o cualquier otro miembro de Electronic Cats)
  a nivel local, y tu has encontrado nuestro código útil ,
  por favor comprar una ronda de cervezas!
  Distribuido como; no se da ninguna garantía.
*/

#include <ArduinoMqttClient.h>
#include <WiFiClientSecure.h> // for MKR1000 change to: #include <WiFi101.h>
#include <ArduinoJson.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

DynamicJsonDocument doc(200);
String json = "";

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiClientSecure.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

WiFiClientSecure wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 8883;
const char topic[]  = "electroniccats/hola";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(4,LOW);
  
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
  Serial.println(topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();

}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // use the Stream interface to print the contents
    while (mqttClient.available()) {
       json = mqttClient.readString();
    }
    Serial.print(json);
    Serial.println();

    demoParse();

    Serial.println();
  }
}

void demoParse() {
  Serial.println("parse");
  Serial.println("=====");

    // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  //JsonObject obj = doc.as<JsonObject>();
  
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
   // Estructura de ejemplo {"led":"ON", "led2": "ON"} o {"led":"OFF", "led2": "OFF"}
   // Fetch values.
  String led = doc["led"];
  String led2 = doc["led2"];
  
  // Print values.
  Serial.println(led);

  if(led == "ON"){
    digitalWrite(4,HIGH);
    Serial.println("Encendido led1");
  }
  if(led == "OFF"){
    digitalWrite(4,LOW);
    Serial.println("Apagado led1");
  }
    if(led2 == "ON"){
    digitalWrite(2,HIGH);
    Serial.println("Encendido led2");
  }
  if(led2 == "OFF"){
    digitalWrite(2,LOW);
    Serial.println("Apagado led2");
  }
}
