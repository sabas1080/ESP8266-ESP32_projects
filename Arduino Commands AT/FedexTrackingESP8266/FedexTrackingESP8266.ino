/************************************************************
FedexTracking.ino
WiFiEsp ESP8266 AT library - Fedex Tracking Example in Temboo
Andres Sabas @ The Inventor's House
Original Creation Date: Jan 13, 2016

This example demonstrates how to use the TCP client 
functionality of the WiFiEsp ESP8266 WiFi library on YalerWebService on 
https://temboo.com

Library required:
//WiFiEsp
https://github.com/bportaluri/WiFiEsp 

****IMPORTANT: Do not forget to set****

// Set profile to use for execution
TrackByNumberChoreo.setProfile("YourProfileTemboo"); // Your Profile, line 102

// Set Choreo inputs
String AccountNumberValue = "YourTrackingNumber"; // Number Tracking Fedex, line 106
    
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
// EspWiFi library uses SoftwareSerial
// to communicate with the ESP8266 module. Include that
// library first:
#include <SoftwareSerial.h>
// Include the ESP8266 AT library:
#include "WiFiEsp.h"
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(8, 9); // RX, TX
#endif

WiFiEspClient client;

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  int status;
  
  Serial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);

if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_SSID, WPA_PASSWORD);
  }
    // Get our assigned IP address and print it:
  Serial.print(F("My IP address is: "));
  Serial.println(WiFi.localIP());

  Serial.println(F("Setup complete.\n"));
}

void loop() {
  if (Serial.available())
  {
    if (numRuns <= maxRuns) {
    Serial.println("Running TrackByNumber - Run #" + String(numRuns++));

    TembooChoreo TrackByNumberChoreo(client);

    // Invoke the Temboo client
    TrackByNumberChoreo.begin();

    // Set Temboo account credentials
    TrackByNumberChoreo.setAccountName(TEMBOO_ACCOUNT);
    TrackByNumberChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    TrackByNumberChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set profile to use for execution
    TrackByNumberChoreo.setProfile("YourProfileTemboo"); // Profile

    // Set Choreo inputs
    String AccountNumberValue = "YourTrackingNumber"; // Number Tracking Fedex
    TrackByNumberChoreo.addInput("AccountNumber", AccountNumberValue);

    // Identify the Choreo to run
    TrackByNumberChoreo.setChoreo("/Library/FedEx/TrackingAndVisibility/TrackByNumber");

    
    // add an output filter to extract the current delivered
    TrackByNumberChoreo.addOutputFilter("delivered", "/SOAP-ENV:Body/CompletedTrackDetails/TrackDetails/StatusDetail/Location/Residential", "Response");

    // Run the Choreo; when results are available, print them to serial
    TrackByNumberChoreo.run();
    
    while(TrackByNumberChoreo.available()) {
      char c = TrackByNumberChoreo.read();
      Serial.print(c);
    }  
    TrackByNumberChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between TrackByNumber calls
  }
}
