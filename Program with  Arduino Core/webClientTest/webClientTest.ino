/*
 ESP32 example: WebClient
 This sketch connects to google website using an ESP32 module to
 perform a simple web search.
 */
 
#include <WiFi.h>

char ssid[] = "Curso";            // your network SSID (name)
char pass[] = "electronicc4ts";        // your network password
int status;     // the Wifi radio's status

char server[] = "arduino.cc";

// Initialize the Wifi client object
WiFiClient client;

void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);

  Serial.println();
  Serial.print(F("connecting to "));
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  // check for the presence of the shield
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  printWifiStatus();

  Serial.println();
  Serial.println("Starting connection to server...");
  
  // if you get a connection, report back via serial
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    client.println("GET /asciilogo.txt HTTP/1.1");
    client.println("Host: arduino.cc");
    client.println("Connection: close");
    client.println();
  }
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();

    // do nothing forevermore
    while (true);
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
