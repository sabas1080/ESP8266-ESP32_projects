/************************************************************
ESP_IFTTT_Maker.ino
IFTTT Channel Maker Posting Example
Andres Sabas @ The Inventor's House
Original Creation Date: Jan 10, 2016

This example demonstrates how to use the TCP client 
functionality of the ESP WiFiSecure library to post 
to a IFTTT event on 
https://ifttt.com

Development environment specifics:
  IDE: Arduino 1.8.9
  Hardware Platform:
  ESP8266 and ESP32

This code is beerware; if you see me (or any other The Inventor's House 
member) at the local, and you've found our code helpful, 
please buy us a round!

Distributed as-is; no warranty is given.
************************************************************/
// Include the ESP8266 and ESP32 WiFiSecure library:
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>

//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
// Replace these two character strings with the name and
// password of your WiFi network.
const char mySSID[] = "YourWiFiName";
const char myPSK[] = "YourWifiPassword";

/////////////////////
// IFTTT Constants //
/////////////////////
// IFTTT destination server:
const char* IFTTTServer = "maker.ifttt.com";
// IFTTT https por:
const int httpsPort = 443;
// IFTTT Event:
const String MakerIFTTT_Event = "button";
// IFTTT private key:
const String MakerIFTTT_Key = "YourPrivateKeyIFTTT";

String httpHeader = "POST /trigger/"+MakerIFTTT_Event+"/with/key/"+MakerIFTTT_Key +" HTTP/1.1\r\n" +
                    "Host: " + IFTTTServer + "\r\n" + 
                    "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
int status;

void setup() 
{
  Serial.begin(9600);
  Serial.println();
  Serial.print(F("connecting to "));
  Serial.println(mySSID);
  WiFi.begin(mySSID, myPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address is: "));
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
  WiFiClientSecure client;
  
  if (client.connect(IFTTTServer, httpsPort) <= 0)
  {
    Serial.println(F("Failed to connect to server."));
    return;
  }
  Serial.println(F("Connected."));

  Serial.println(F("Posting to IFTT Event!"));
  
  client.print(httpHeader);

  // available() will return the number of characters
  // currently in the receive buffer.
  while (client.available())
    Serial.write(client.read()); // read() gets the FIFO char
  
  // connected() is a boolean return value - 1 if the 
  // connection is active, 0 if it's closed.
  if (client.connected())
    client.stop(); // stop() closes a TCP connection.
}
