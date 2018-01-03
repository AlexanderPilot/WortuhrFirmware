/* Einbinden von Bibliotheken */
#include "Configuration.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EasyNTPClient.h>
#include <WiFiUdp.h>

/* Anlegen von globalen Variablen */

/* Matrix als Bildschirmspeicher */
word Matrix[11];

/* Telnet Server */
WiFiServer server(TELNET_PORT);
WiFiClient serverClients[MAX_SRV_CLIENTS];
int Buffer;

/* UDP Server */
WiFiUDP udp;

/* NTP Client */
EasyNTPClient ntpClient(udp, "de.pool.ntp.org", ((0*60*60)+(0*60))); // +Stunden +Minuten zur GMT

/* TimerSetups */
#define LED D6
bool toggle = false;
long UNIXZeit;

void timer0_ISR (void) {
  if (toggle) {
    digitalWrite(LED, HIGH);
    toggle = false;
  } else {
    digitalWrite(LED, LOW);
    toggle = true;
  }
  //timer0_write(ESP.getCycleCount() + 80000000L); // 80MHz == 1sec
  
}

void setup()
{
  Serial.begin(SERIAL_SPEED);
  _SERIAL_BEGIN(SERIAL_SPEED);
  delay(10);
  Serial.println("");
  Serial.println("ESP8266 Wortuhr");
  Serial.print("Version: ");
  Serial.println(FIRMWARE_VERSION);
  
  /* Initialisieren der seriellen Schnittstelle */
  _SERIAL_PRINTLN("");
  _SERIAL_PRINTLN("Starte Initialisierung");
  _SERIAL_PRINTLN("");

  /* Initialisierung des WLAN Moduls */
  WiFi.mode(WIFI_AP_STA);
  _SERIAL_PRINTLN("Starte Wifi Dual Mode");
  _SERIAL_PRINTLN("STA mode");
  WiFi.begin(sta_ssid, sta_password);
  _SERIAL_PRINT("Connecting to ");
  _SERIAL_PRINT(sta_ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    _SERIAL_PRINT(".");
  }
  _SERIAL_PRINTLN("finished");
  _SERIAL_PRINTLN("STA mode initialisiert");

  _SERIAL_PRINTLN("AP mode");
  WiFi.softAP(ap_ssid, ap_password);
  IPAddress myIP = WiFi.softAPIP();
  _SERIAL_PRINT("IP-Adresse: ");
  _SERIAL_PRINTLN(myIP);
  _SERIAL_PRINTLN("AP mode initialisiert");

  _SERIAL_PRINTLN("Starte Server");
  server.begin();
  server.setNoDelay(true);
  _SERIAL_PRINTLN("abgeschlossen");

  pinMode(LED, OUTPUT);
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(timer0_ISR);
  timer0_write(ESP.getCycleCount() + 80000000L); // 80MHz == 1sec
  interrupts();

//-----------------------------------------

  //_SERIAL_PRINTLN("Initialisierung des WLAN Moduls abgeschlossen");




  /* Initialisierung der RTC */
  //_SERIAL_PRINTLN("Initialisierung der RTC abgeschlossen");


  /* Initialisierung der LED Streifen */
  //_SERIAL_PRINTLN("Initialisierung der WS2812B LED Streifen abgeschlossen");


  /* Initialisierung der GPIO Pins */
  pinMode(LED_SVR_HAS_CLNT, OUTPUT);
  digitalWrite(LED_SVR_HAS_CLNT, HIGH);
  delay(500);
  digitalWrite(LED_SVR_HAS_CLNT, LOW);

  pinMode(D7, OUTPUT);
  digitalWrite(D7, LOW);

  _SERIAL_PRINTLN("Initialisierung der GPIO Ports abgeschlossen");


  //_SERIAL_PRINTLN("Initialisierung der Wortuhr abgeschlossen");
  Serial.println("Ausgabe der UNIX Time");
}

void loop()
{
  UNIXZeit = ntpClient.getUnixTime();
  //Serial.println(ntpClient.getUnixTime());
  delay(1000);
//  uint8_t i;
//  //check if there are any new clients
//  if (server.hasClient()) {
//    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
//      //find free/disconnected spot
//      if (!serverClients[i] || !serverClients[i].connected()) {
//        if (serverClients[i]) serverClients[i].stop();
//        serverClients[i] = server.available();
//        continue;
//      }
//    }
//    //no free/disconnected spot so reject
//    WiFiClient serverClient = server.available();
//    serverClient.stop();
//  }
//  //check clients for data
//  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
//    if (serverClients[i] && serverClients[i].connected()) {
//      if (serverClients[i].available()) {
//        //get data from the telnet client and push it to the UART
//        while (serverClients[i].available())
//        {
//          Buffer = serverClients[i].read();
//          _SERIAL_WRITE(Buffer);
//          if (Buffer == 'a')
//          {
//            digitalWrite(D6, HIGH);
//          }
//          else if (Buffer == 'b')
//          {
//            digitalWrite(D6, LOW);
//          }
//          else
//          {
//            //nop
//          }
//        }
//      }
//    }
//  }
//  //check UART for data
//  if (Serial.available()) {
//    size_t len = Serial.available();
//    uint8_t sbuf[len];
//    Serial.readBytes(sbuf, len);
//    //push UART data to all connected telnet clients
//    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
//      if (serverClients[i] && serverClients[i].connected()) {
//        serverClients[i].write(sbuf, len);
//        delay(1);
//        if (serverClients[i].read() == 1)
//        {
//          digitalWrite(D6, HIGH);
//        }
//        else
//        {
//          digitalWrite(D6, LOW);
//        }
//      }
//    }
//  }
}
