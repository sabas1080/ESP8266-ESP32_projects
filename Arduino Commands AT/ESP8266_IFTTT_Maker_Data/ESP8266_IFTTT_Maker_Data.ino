/************************************************************
ESP8266_IFTTT_Maker_Data.ino
WiFiEsp ESP8266 AT library - IFTTT Channel Maker Posting Data Example
Andres Sabas @ The Inventor's House
Original Creation Date: Jan 10, 2016
https://github.com/bportaluri/WiFiEsp

This example demonstrates how to use the TCP client 
functionality of the WiFiEsp ESP8266 WiFi library to post 
data to a IFTTT event on 
https://ifttt.com

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
// Include the ESP8266 AT library:
#include "WiFiEsp.h"


// Emulate Serial1 on pins 8/9 if not present
// The WiFiEsp library uses SoftwareSerial
// to communicate with the ESP8266 module. Include that
// library first:
#ifndef HAVE_HWSERIAL1
#include <SoftwareSerial.h>
SoftwareSerial Serial1(8, 9); // RX, TX
#endif

//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
// Replace these two character strings with the name and
// password of your WiFi network.
// password of your WiFi network.
#define mySSID "YourWifiName"
#define myPSK "YourWifiPassword"

/////////////////////
// IFTTT Constants //
/////////////////////
// IFTTT detsination server:
#define IFTTTServer "maker.ifttt.com"
// IFTTT Event:
#define MakerIFTTT_Event "button"
// IFTTT private key:
#define MakerIFTTT_Key "YourPrivateKeyIFTTT"

#define httpHeader "POST /trigger/" MakerIFTTT_Event "/with/key/" MakerIFTTT_Key " HTTP/1.1\r\n" \
                    "Host: " IFTTTServer "\r\n" \ 
                    "Content-Type: application/json\r\n"

                    
void setup() 
{
  int status = WL_IDLE_STATUS; // the Wifi radio's status
  Serial.begin(9600);

  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);
  
 // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(mySSID);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(mySSID, myPSK);
  }
  
  // Get our assigned IP address and print it:
  Serial.print(F("My IP address is: "));
  Serial.println(WiFi.localIP());
  
  Serial.println(F("Press any key to post to IFTTT!"));
}

void loop()
{
  // If a character has been received over serial:
  if (Serial.available())
  {
    // !!! Make sure we haven't posted recently
    // Post to IFTTT!
    postToIFTTT();
    // Then clear the serial buffer:
    while (Serial.available())
      Serial.read();
  }
}

void postToIFTTT()
{
  // Create a client, and initiate a connection
  WiFiEspClient client;
  
  if (client.connect(IFTTTServer, 80) <= 0)
  {
    Serial.println(F("Failed to connect to server."));
    return;
  }
  Serial.println(F("Connected."));
  
  // Set up our IFTTT post parameters:
  String params1, params2,params3;
  params1 = String(analogRead(A0));
  params2 = String(analogRead(A1));
  params3 = String(analogRead(A2));

  String data="{\"value1\":\""+params1+"\",\"value2\":\""+params1+"\",\"value3\":\""+params1+"\"}";
  
  Serial.println(F("Posting to IFTTT!"));
  //Serial.println(data.length());
  
  client.print(httpHeader);
  client.print("Content-Length: "); client.print(data.length());
  client.print("\r\n\r\n");
  client.print(data);

  // available() will return the number of characters
  // currently in the receive buffer.
  while (client.available())
    Serial.write(client.read()); // read() gets the FIFO char
  
  // connected() is a boolean return value - 1 if the 
  // connection is active, 0 if it's closed.
  if (client.connected())
    client.stop(); // stop() closes a TCP connection.
}
