/* Einbinden von Bibliotheken */
#include "Configuration.h"
#include <Wire.h>
#include "DS3231.h"
#include "Renderer.h"
#include <stddef.h>
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "globals.h"
#include "Lights.h"
#include <EEPROM.h>
#include "StorageData.h"

/* Anlegen der Objekte*/
Renderer renderer;
Lights lights;
DS3231 ds3231(DS3231_ADDRESS);
WiFiServer server(TELNET_PORT);
WiFiClient serverClients[MAX_SRV_CLIENTS];

/* Matrix als Bildschirmspeicher */
word Matrix[11];

/* WiFi Configurations */
const char *sta_ssid     = "heikach";
const char *sta_password = "1990augsburGMHVD!";
//const char *sta_ssid     = "UPC68EE18B";
//const char *sta_password = "Tw11tYbolz@#";
//const char *sta_ssid     = "heikach";
//const char *sta_password = "";


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WS2812B_PIN, NEO_GRB + NEO_KHZ800);



void setup()
{
  //-----------------------------------------------------------
  //Definition von Variablen
  uint8_t counterWiFiConnection = 0;
  uint8_t maxValCntWiFi = 50;

  //-----------------------------------------------------------
  //Oeffnen der UART Ausgabe(aktivierte odear deaktivierte Debugschnittstelle)
  Serial.begin(SERIAL_SPEED);
  _DEBUG_BEGIN(SERIAL_SPEED);
  //Zeitverzug um serielle Kommunikation sicher aufgebaut zu haben
  delay(10);

  //Begruessungstext inkl. Versionsnummer
  _DEBUG_PRINTLN("Starte Initialisierung");
  _DEBUG_PRINTLN("");

  //-----------------------------------------------------------
  //Initialisierung Dual Modus
  WiFi.mode(WIFI_STA); //Zuvor: WIFI_AP_STA
  WiFi.begin(sta_ssid,sta_password);
  _DEBUG_PRINTLN("Starte Wifi STA Mode");

  //-----------------------------------------------------------
  //Initialisierung Access Point Modus
  WiFi.config(serv_ip,serv_gateway,serv_subnet);
  //WiFi.softAP(ap_ssid, ap_password);

  //-----------------------------------------------------------

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
      //Reaktion, da sich nicht ins WLAN eingewaehlt werden kann
      //ggfs Meldung an App falls diese Verbunden ist
      //Ausgabe einer Fehlermeldung ueber die Apps
    }
  }
  _DEBUG_PRINTLN("finished");
  _DEBUG_PRINTLN("STA mode initialisiert");

  //-----------------------------------------------------------
  //I2C initialisieren
  //_DEBUG_PRINTLN("");
  //_DEBUG_PRINTLN("I2C Bus aktivieren");

  //Wire.begin(D1, D2); //SDA, SCL
  //ds3231.printRTCTime();

  //-----------------------------------------------------------

  //start server
    server.begin();
    server.setNoDelay(true);

  //GPIOs konfigurieren
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);

  // This initializes the NeoPixel library.
  // ----------------------------------------------------------
  pixels.begin();
}

void loop()
{
  //-----------------------------------------------------------
  //Variablendefinition
  char resChar;
  uint8_t i;
  //ds3231.printRTCTime();

  //-----------------------------------------------------------
  //Pruefen ob neuer Client vorhanden
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
        _DEBUG_PRINT("New client: " + i);
        delay(1);
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
        while ( serverClients[i].available())
        {

        	resChar = serverClients[i].read();
        	_SRVCOMM_WRITE( resChar );

        	// Hauptteil zum Einstellen der LEDs
        	if( lights.comevatiation( resChar ) ){
        		// TODO: Hier muss die endgueltige Steuerung der LEDs eingefuegt werden
        		for(int i=0;i<NUMPIXELS;i++){
        			pixels.setPixelColor(i, pixels.Color(farbanteile.red,farbanteile.green,farbanteile.blue));
        			pixels.show(); // This sends the updated pixel color to the hardware.
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
