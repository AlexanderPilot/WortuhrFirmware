/* Einbinden von Bibliotheken */
#include "Configuration.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "DS3231.h"
#include "App_Interpreter.h"
#include "LED_Ausgabe.h"
#include "Renderer.h"
#include "Settings.h"
//#include "Server.h"

/* Anlegen der Objekte*/
Settings settings;
Renderer renderer;
LED_Ausgabe led_ausgabe;
DS3231 ds3231(DS3231_ADDRESS);
WiFiServer server(TELNET_PORT);
WiFiClient serverClients[MAX_SRV_CLIENTS];

/* Matrix als Bildschirmspeicher */
word Matrix[11];

/* WiFi Configurations */
const char *sta_ssid     = "ASUS";
const char *sta_password = "Br8#Pojg56";
//const char *sta_ssid     = "UPC68EE18B";
//const char *sta_password = "Tw11tYbolz@#";
//const char *sta_ssid     = "heikach";
//const char *sta_password = "";

/* Globale Variablen */
int Buffer;


void setup()
{
  //-----------------------------------------------------------
  //Definition von Variablen
  uint8_t counterWiFiConnection = 0;
  uint8_t maxValCntWiFi = 50;

  //-----------------------------------------------------------
  //Öffnen der UART Ausgabe(aktivierte oder deaktivierte Debugschnittstelle)
  Serial.begin(SERIAL_SPEED);
  _DEBUG_BEGIN(SERIAL_SPEED);
  //Zeitverzug um serielle Kommunikation sicher aufgebaut zu haben
  delay(10);
  //Begrüßungstext inkl. Versionsnummer
  Serial.println("");
  Serial.println("ESP8266 Wortuhr");
  Serial.print("Version: ");
  Serial.println(FIRMWARE_VERSION);
  _DEBUG_PRINTLN("");
  _DEBUG_PRINTLN("Starte Initialisierung");
  _DEBUG_PRINTLN("");

  //-----------------------------------------------------------
  //Initialisierung Dual Modus  
  WiFi.mode(WIFI_AP_STA);
  _DEBUG_PRINTLN("Starte Wifi Dual Mode");
  _DEBUG_PRINTLN("");
  
  //-----------------------------------------------------------
  //Initialisierung Access Point Modus
  _DEBUG_PRINTLN("");
  _DEBUG_PRINTLN("1 - AP mode");
  WiFi.softAPConfig(ap_ip, ap_gateway, ap_subnet);
  WiFi.softAP(ap_ssid, ap_password);
  _DEBUG_PRINT("IP-Adresse: ");
  _DEBUG_PRINTLN(ap_ip);
  _DEBUG_PRINTLN("AP mode initialisiert");
  
  //-----------------------------------------------------------
  //Initialisierung Stationärer Modus
  _DEBUG_PRINTLN("2 - STA mode");
  //Starten der WiFi Verbindung zum stationären Netz
  WiFi.begin(sta_ssid, sta_password);
  //Konfiguration der IP-Adresse
  //WiFi.config(ip, gateway, subnet);
  _DEBUG_PRINT("Connecting to ");
  _DEBUG_PRINT(sta_ssid);

  while ((WiFi.status() != WL_CONNECTED) && (counterWiFiConnection < maxValCntWiFi))
  {
    delay(500);
    _DEBUG_PRINT(".");
    counterWiFiConnection++;
  }

  if (counterWiFiConnection == maxValCntWiFi)
  {
    _DEBUG_PRINT("Could not connect to ");
    _DEBUG_PRINT(sta_ssid);
    while (1)
    {
      //Reaktion, da sich nicht ins WLAN eingewählt werden kann
      //ggfs Meldung an App falls diese Verbunden ist
      //Ausgabe einer Fehlermeldung über die Apps
    }
  }
  _DEBUG_PRINTLN("finished");
  _DEBUG_PRINTLN("STA mode initialisiert");

  //-----------------------------------------------------------
  //Starten des Servers
  _DEBUG_PRINTLN("");
  _DEBUG_PRINTLN("Starte den Telnet Server");
  server.begin();
  server.setNoDelay(true);
  _DEBUG_PRINTLN("Server läuft");

  _DEBUG_PRINTLN("");
  _DEBUG_PRINTLN("Verbindungsaufbau über:");
  _DEBUG_PRINT("SSID: ");
  _DEBUG_PRINTLN(ap_ssid);
  _DEBUG_PRINT("IP-Adresse: ");
  _DEBUG_PRINTLN(ap_ip);
  _DEBUG_PRINT("Port: ");
  _DEBUG_PRINTLN("23");

  //-----------------------------------------------------------
  //I2C initialisieren
  _DEBUG_PRINTLN("");
  _DEBUG_PRINTLN("I2C Bus aktivieren");
  
  Wire.begin(D1, D2); //SDA, SCL
  ds3231.printRTCTime();

  //-----------------------------------------------------------
  //GPIOs konfigurieren
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(LED_SVR_HAS_CLNT, OUTPUT);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
  digitalWrite(LED_SVR_HAS_CLNT, HIGH);
}

void loop()
{
  //-----------------------------------------------------------
  //Variablendefinition
  char myChar;
  uint8_t i;
  ds3231.printRTCTime();

  //-----------------------------------------------------------
  //Prüfen ob neuer Client vorhanden
  if (server.hasClient())
  {
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected())
      {
        if (serverClients[i])
        {
          serverClients[i].stop();
        }
        serverClients[i] = server.available();
        _DEBUG_PRINT("New client: ");
        _DEBUG_PRINTLN(i);
        digitalWrite(LED_SVR_HAS_CLNT, LOW);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }

  //-----------------------------------------------------------
  //check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++)
  {
    if (serverClients[i] && serverClients[i].connected())
    {
      if (serverClients[i].available())
      {
        //get data from the telnet client and push it to the UART
        while (serverClients[i].available())
        {
          myChar = serverClients[i].read();
          _SRVCOMM_WRITE(myChar);
          if (myChar == 'X')
          {
            if (digitalRead(D6))
            {
              digitalWrite(D6, LOW);
            }
            else
            {
              digitalWrite(D6, HIGH);
            }
          }
          if (myChar == 'Y')
          {
            if (digitalRead(D7))
            {
              digitalWrite(D7, LOW);
            }
            else
            {
              digitalWrite(D7, HIGH);
            }
          }
        }
      }
    }
  }
  //check UART for data
  if (Serial.available())
  {
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      if (serverClients[i] && serverClients[i].connected())
      {
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }
}
