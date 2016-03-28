// Copyright (c) 2015, Yaler GmbH, Switzerland
// All rights reserved

/************************************************************
YalerWebService.ino
WiFiEsp ESP8266 AT library - Yaler Web Service Example
Andres Sabas @ The Inventor's House
Original Creation Date: Jan 9, 2016

This example demonstrates how to use the TCP client 
functionality of the WiFiEsp ESP8266 WiFi library on YalerWebService on 
//https://yaler.net

Library required:
WiFiEsp
//https://github.com/bportaluri/WiFiEsp 

YalerEspServer
//https://github.com/sabas1080/YalerWiFiEspServer 

Development environment specifics:
  IDE: Arduino 1.6.5
  Hardware Platform: Arduino Uno
  ESP8266 V1 4M
  or ESP8266 WiFi Shield Version: 1.0

This code is beerware; if you see me (or any other The Inventor's House 
member) at the local, and you've found our code helpful, 
please buy us a round!

Distributed as-is; no warranty is given.
************************************************************/

#include "WiFiEsp.h"
#include <YalerWiFiServer.h>

// Emulate Serial1 on pins 7/6 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(8, 9); // RX, TX
#endif

char ssid[] = "YourWiFiName"; 
char pass[] = "YourWiFiPassword";

int status = WL_IDLE_STATUS;

// Local WiFiServer at http://LOCAL_IP/ (e.g. http://192.168.0.7/)
//WiFiServer server(80);

// Get a relay domain at https://yaler.net/ to replace RELAY_DOMAIN below
// The YalerWiFiServer is accessible at http://RELAY_DOMAIN.try.yaler.io/
YalerWiFiServer server("try.yaler.io", 80, "RELAY_DOMAIN");

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);
  
  Serial.println("Acquiring IP address...");
  if (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Connecting to WiFi failed.");
  } else {
    Serial.println(WiFi.localIP());
    server.begin();
  }
}

void sendResponse(WiFiEspClient client) {
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Connection: close\r\n");
  client.print("Content-Length: 5\r\n");
  client.print("\r\n");
  client.print("Sabas");
}

void loop() {
  WiFiEspClient client = server.available();
  if (client && client.connected()) {
    client.find("\r\n\r\n"); // Consume incoming request
    sendResponse(client);
    delay(3000); // Give the Web browser time to receive the data
    client.stop();
  }
}
