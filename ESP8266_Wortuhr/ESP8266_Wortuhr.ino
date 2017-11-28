/* Einbinden von Bibliotheken */
#include "Configuration.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

/* Anlegen von globalen Variablen */

/* Matrix als Bildschirmspeicher */
word Matrix[11];

/* WiFi Settings*/
const char *ssid = AP_SSID;
const char *password = AP_PW;

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

  /* Initialisierung des WLAN Moduls */
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
#if DEBUG == true
  Serial.println("Initialisierung des WLAN Moduls abgeschlossen");
#endif

  /* Initialisierung des Telnet Servers */
  server.begin();
  server.setNoDelay(true);
#if DEBUG == true
  Serial.println("Initialisierung des Telnet Servers abgeschlossen");
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
  digitalWrite(LED_SVR_HAS_CLNT, LOW);
  Serial.println("Initialisierung der GPIO Ports abgeschlossen");

#if DEBUG == true
  Serial.println("Initialisierung der Wortuhr abgeschlossen");
#endif
}

void loop()
{
  /* Anlegen von Variablen */
  uint8_t i;

  /* Wenn Client verbunden ist, soll LED angehen */
  if (server.hasClient())
  {
    digitalWrite(LED_SVR_HAS_CLNT, HIGH);
  }
  else
  {
    digitalWrite(LED_SVR_HAS_CLNT, LOW);
  }

  /* Prüfen ob neuer Client verfügbar */
  if (server.hasClient())
  {
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected())
      {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }

  /* prüfen ob der Client neue Daten versendet hat */
  for (i = 0; i < MAX_SRV_CLIENTS; i++)
  {
    if (serverClients[i] && serverClients[i].connected())
    {
      if (serverClients[i].available())
      {
        //get data from the telnet client and push it to the UART
        while (serverClients[i].available())
        {
          Buffer = serverClients[i].read();
          Serial.write(Buffer);
          if (Buffer == 'a')
          {
            digitalWrite(LED_SVR_HAS_CLNT, HIGH);
          }
          else if (Buffer == 'b')
          {
            digitalWrite(LED_SVR_HAS_CLNT, LOW);
          }
          else
          {
            //nop
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
  /* Einlesen der Uhrzeit aus NTP Server oder App*/

  /* Speichern der Uhrzeit auf RTC */

  /* Ausgabe der Buchstaben auf LEDs */

  /* Ausgabe von Debug-Nachrichten über die serielle Schnittstelle */

}
