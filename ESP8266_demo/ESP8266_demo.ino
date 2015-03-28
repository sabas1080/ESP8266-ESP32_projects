/*
*Andres Sabas
*Sketch para comprobar funcionamiento del ESP8266
*Firmware de ESP8266: 0.9.5
*Fecha: 14 de Febrero del 2015
*/
#include <SoftwareSerial.h>
#define SSID "SSDI_Nombre de Red"
#define PASS "Clave-Wifi"
#define DST_IP "www.google.com"

   SoftwareSerial dbgSerial(10, 11); // RX, TX
   void setup()
   {
     // Open serial communications and wait for port to open:
     Serial.begin(9600);
     //Serial.setTimeout(5000);
     dbgSerial.begin(9600); //can't be faster than 9600 for softserial
     Serial.println("ESP8266 Demo");
     //test if the module is ready
    /* dbgSerial.println("AT+RST");
     delay(500);
     if(dbgSerial.find("Ready"))
     {
       Serial.println("Modulo Funcionando");
     }
     else
     {
       Serial.println("Modulo no detectado o no funcionando.");
       //while(1);
     }
     delay(1000);*/
     Serial.println("Intento Wifi");
     //connect to the wifi
     boolean connected=false;
     for(int i=0;i<5;i++)
     {
       if(connectWiFi())
       {
         connected = true;
         break;
       }
     }
     if (!connected){while(1);}
     delay(5000);
     //print the ip addr
     dbgSerial.println("AT+CIFSR");
     Serial.println("ip address:");
     while (dbgSerial.available())
     Serial.write(dbgSerial.read());
     //set the single connection mode
     dbgSerial.println("AT+CIPMUX=0");
   }
   void loop()
   {
     String cmd = "AT+CIPSTART=\"TCP\",\"";
     cmd += DST_IP;
     cmd += "\",80";
     dbgSerial.println(cmd);
     Serial.println(cmd);
     if(dbgSerial.find("Error")) return;
     cmd = "GET / HTTP/1.0\r\n\r\n";
     dbgSerial.print("AT+CIPSEND=");
     dbgSerial.println(cmd.length());
     if(dbgSerial.find(">"))
     {
       Serial.print(">");
       }else
       {
         dbgSerial.println("AT+CIPCLOSE");
         Serial.println("connect timeout");
         delay(1000);
         return;
       }
       dbgSerial.print(cmd);
       delay(2000);
       //Serial.find("+IPD");
       while (dbgSerial.available())
       {
         char c = dbgSerial.read();
         Serial.write(c);
         if(c=='\r') Serial.print('\n');
       }
       Serial.println("====");
       delay(1000);
     }
     boolean connectWiFi()
     {
       dbgSerial.println("AT+CWMODE=1");
       String cmd="AT+CWJAP=\"";
       cmd+=SSID;
       cmd+="\",\"";
       cmd+=PASS;
       cmd+="\"";
       Serial.println(cmd);
       dbgSerial.println(cmd);
       delay(5000);
       if(dbgSerial.find("OK"))
       {
         Serial.println("OK, Connectado a WiFi.");
         delay(5000);
         return true;
         }else
         {
           Serial.println("No se pudo conectar al WiFi.");
           return false;
         }
       }
