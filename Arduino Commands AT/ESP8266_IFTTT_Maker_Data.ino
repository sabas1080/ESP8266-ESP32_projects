/************************************************************
ESP8266_IFTTT_Maker_Data.ino
SparkFun ESP8266 AT library - IFTTT Channel Maker Posting Example
Andres Sabas @ The Inventor's House
Original Creation Date: Jan 10, 2016
https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library

This example demonstrates how to use the TCP client 
functionality of the SparkFun ESP8266 WiFi library to post 
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
// The SparkFunESP8266WiFi library uses SoftwareSerial
// to communicate with the ESP8266 module. Include that
// library first:
#include <SoftwareSerial.h>
// Include the ESP8266 AT library:
#include <SparkFunESP8266WiFi.h>

//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
// Replace these two character strings with the name and
// password of your WiFi network.
const char mySSID[] = "YourName Wifi";
const char myPSK[] = "PasswordWiFi";

/////////////////////
// IFTTT Constants //
/////////////////////
// IFTTT destination server:
const String IFTTTServer = "maker.ifttt.com";
// IFTTT Name Event:
const String MakerIFTTT_Event = "test";
// IFTTT private key:
const String MakerIFTTT_Key = "KeyPrivateIFTTT";

String httpHeader = "POST /trigger/"+MakerIFTTT_Event+"/with/key/"+MakerIFTTT_Key +" HTTP/1.1\r\n" +
                    "Host: " + IFTTTServer + "\r\n" + 
                    "Content-Type: application/json\r\n";

                    
void setup() 
{
  int status;
  Serial.begin(9600);
  
  // To turn the MG2639 shield on, and verify communication
  // always begin a sketch by calling cell.begin().
  status = esp8266.begin();
  if (status <= 0)
  {
    Serial.println(F("Unable to communicate with shield. Looping"));
    while(1) ;
  }
  
  esp8266.setMode(ESP8266_MODE_STA); // Set WiFi mode to station
  if (esp8266.status() <= 0) // If we're not already connected
  {
    if (esp8266.connect(mySSID, myPSK) < 0)
    {
      Serial.println(F("Error connecting"));
      while (1) ;
    }    
  }
  
  // Get our assigned IP address and print it:
  Serial.print(F("My IP address is: "));
  Serial.println(esp8266.localIP());
  
  Serial.println(F("Press any key to post to IFTTT!"));
}

void loop()
{
  // If a character has been received over serial:
  if (Serial.available())
  {
    // !!! Make sure we haven't posted recently
    // Post to IFTTT!
    postToPhant();
    // Then clear the serial buffer:
    while (Serial.available())
      Serial.read();
  }
}

void postToPhant()
{
  // Create a client, and initiate a connection
  ESP8266Client client;
  
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
