/* Einbinden von Bibliotheken */
#include "Configuration.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

/* Anlegen von globalen Variablen */

/* Matrix als Bildschirmspeicher */
word Matrix[11];

/* WiFi Settings*/
const char *ap_ssid = AP_SSID;
const char *ap_password = AP_PW;

const char* sta_ssid = "";
const char* sta_password = "";

/* Telnet Server */
WiFiServer server(TELNET_PORT);
WiFiClient serverClients[MAX_SRV_CLIENTS];
int Buffer;

void setup()
{
  /* Initialisieren der seriellen Schnittstelle */
  Serial.begin(SERIAL_SPEED);     //Öffnen der seriellen Verbindung mit der Geschwindigkeit SERIAL_SPEED
  delay(10);                      //warten, dass serielle Verbindung sicher hergestellt ist
  Serial.println("ESP8266 Wortuhr");
  Serial.println("");
  Serial.print("Version: ");
  Serial.println(FIRMWARE_VERSION);
  Serial.println("");
  Serial.println("Starte Initialisierung");
  Serial.println("");

  /* Initialisierung des WLAN Moduls */
  WiFi.mode(WIFI_MODE);
  Serial.println(WIFI_MODE);
  //-----------------------------------------
#if WIFI_MODE == 1
#if DEBUG == true
  Serial.println("WiFi Stationary");
#endif
  //Settings for STA Mode
  WiFi.begin(sta_ssid, sta_password);

#if DEBUG == true
  Serial.println("WiFi Stationary");
#endif
#endif
  //-----------------------------------------
#if WIFI_MODE == 2
#if DEBUG == true
  Serial.println("WiFi Acccess Point");
#endif
  //Settings for AP Mode
  WiFi.softAP(ap_ssid, ap_password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP-Adresse: ");
  Serial.println(myIP);
  server.begin();
  server.setNoDelay(true);

#if DEBUG == true
  Serial.println("WiFi Acccess Point");
#endif
#endif
  //-----------------------------------------
#if WIFI_MODE == 3
#if DEBUG == true
  Serial.println("WiFi Dual Mode");
#endif
  //Settings for Dual Mode


#if DEBUG == true
  Serial.println("WiFi Dual Mode");
#endif
#endif
//-----------------------------------------

#if DEBUG == true
  Serial.println("Initialisierung des WLAN Moduls abgeschlossen");
#endif



  /* Initialisierung der RTC */
#if DEBUG == true
  Serial.println("Initialisierung der RTC abgeschlossen");
#endif

  /* Initialisierung der LED Streifen */
#if DEBUG == true
  Serial.println("Initialisierung der WS2812B LED Streifen abgeschlossen");
#endif

  /* Initialisierung der GPIO Pins */
  pinMode(LED_SVR_HAS_CLNT, OUTPUT);
  digitalWrite(LED_SVR_HAS_CLNT, HIGH);
  delay(500);
  digitalWrite(LED_SVR_HAS_CLNT, LOW);

  pinMode(D7, OUTPUT);
  digitalWrite(D7, HIGH);

#if DEBUG == true
  Serial.println("Initialisierung der GPIO Ports abgeschlossen");
#endif

  Serial.println("Initialisierung der Wortuhr abgeschlossen");

}

void loop() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        //get data from the telnet client and push it to the UART
        while (serverClients[i].available())
        {
          Buffer = serverClients[i].read();
          Serial.write(Buffer);
          if (Buffer == 'a')
          {
            digitalWrite(D6, HIGH);
          }
          else if (Buffer == 'b')
          {
            digitalWrite(D6, LOW);
          }
          else
          {
            //nop
          }
        }
      }
    }
  }
  //check UART for data
  if (Serial.available()) {
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (serverClients[i] && serverClients[i].connected()) {
        serverClients[i].write(sbuf, len);
        delay(1);
        if (serverClients[i].read() == 1)
        {
          digitalWrite(D6, HIGH);
        }
        else
        {
          digitalWrite(D6, LOW);
        }
      }
    }
  }
}
