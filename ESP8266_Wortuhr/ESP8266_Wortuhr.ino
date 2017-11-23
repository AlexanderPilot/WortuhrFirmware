/* Einbinden von Bibliotheken */
#include "Configuration.h"

/* Anlegen von globalen Variablen */

/* Matrix als Bildschirmspeicher */
word Matrix[11];

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
  
  #if DEBUG == true
  Serial.println("Initialisierung der Wortuhr abgeschlossen"); 
  #endif
}

void loop()
{
  /* Einlesen der Uhrzeit aus NTP Server oder App*/

  /* Speichern der Uhrzeit auf RTC */

  /* Ausgabe der Buchstaben auf LEDs */

  /* Ausgabe von Debug-Nachrichten über die serielle Schnittstelle */
  
}
