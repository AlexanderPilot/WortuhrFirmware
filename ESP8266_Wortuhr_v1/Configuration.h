/* Headerdatei für Konfigurationen */

/* Vermeidung Doppeldefinitionen */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* Serielle Geschwindigkeit */
#define SERIAL_SPEED 74880

/* Versionsstand */
#define FIRMWARE_VERSION "1.0"

/* Debug Botschaften */
//#define DEBUG_PRINT

#ifdef DEBUG_PRINT
  #define _SERIAL_BEGIN(x)    Serial.begin(x);
  #define _SERIAL_PRINT(x)    Serial.print(x);
  #define _SERIAL_PRINTLN(x)  Serial.println(x);
  #define _SERIAL_WRITE(x)    Serial.write(x);
#else
  #define _SERIAL_BEGIN(x)
  #define _SERIAL_PRINT(x)
  #define _SERIAL_PRINTLN(x)
  #define _SERIAL_WRITE(x)
#endif

/* WiFi AP IP-Addresse */
#define AP_IP_ADDR ""

/* Telnet Server Port */
#define TELNET_PORT 23

/* Clients auf Server */
#define MAX_SRV_CLIENTS 1

/* LED für Client-Verbindung zum Server */
#define LED_SVR_HAS_CLNT D6

/* WiFi Settings*/
const char *ap_ssid = "Wordclock";
const char *ap_password = "Wordclock";

const char* sta_ssid = "ASUS";
const char* sta_password = "Br8#Pojg56";

#endif
