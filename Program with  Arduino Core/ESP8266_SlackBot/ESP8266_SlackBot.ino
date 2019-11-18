/************************************************************
ESP866_SlackBot.ino
SlackBot Posting Example
Andres Sabas @ The Inventor's House
Original Creation Date: Jan 10, 2016

This example demonstrates how to use the TCP client
functionality of the ESP8266 WiFiSecure library to post
data to a IFTTT event on
https://slack.com

Development environment specifics:
  IDE: Arduino 1.6.5
  Hardware Platform:
  ESP8266 HUZZAH Adafruit
  or NodeMCU: 1.0

This code is beerware; if you see me (or any other The Inventor's House
member) at the local, and you've found our code helpful,
please buy us a round!

Distributed as-is; no warranty is given.
************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "YourWiFiName";
const char* password = "YourWiFiPassword";

const char* host = "YourSlack.slack.com";
const int httpsPort = 443;
//SlackChannel Post
const String SlackChannel = "random";
// SlackBot private Token:
const String Slack_Token = "YourSlackBotToken";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String data = "Hola";
  String url = "/services/hooks/slackbot?token="+Slack_Token+"&channel=%23"+SlackChannel;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n");
  client.print("Content-Length: "); client.print(data.length());
  client.print("\r\n\r\n");
  client.print(data);
  client.print("Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}

void loop() {
}
