/* Einbinden von Bibliotheken */
#include "Configuration.h"

/* Anlegen von globalen Variablen */

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

  /* Initialisierung der RTC */
  
  /* Initialisierung der LED Streifen */

  /* Ausgabe von Debug-Nachtichten über die serielle Schnittstelle */
  #if DEBUG == true
  Serial.println("Initialisierung abgeschlossen"); 
  #endif
}

void loop()
{
  /* Einlesen der Uhrzeit aus NTP Server oder App*/

  /* Speichern der Uhrzeit auf RTC */

  /* Ausgabe der Buchstaben auf LEDs */

  /* Ausgabe von Debug-Nachrichten über die serielle Schnittstelle */
  
}
