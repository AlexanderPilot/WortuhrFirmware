/* Headerdatei für Konfigurationen */

/* Vermeidung Doppeldefinitionen */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* Weitere Bibliotheken */
#include <ESP8266WiFi.h>

/* Serielle Geschwindigkeit */
#define SERIAL_SPEED 74880

/* Versionsstand */
#define FIRMWARE_VERSION "1.0"

/* Debug Botschaften */
#define DEBUG_PRINT

#ifdef DEBUG_PRINT
  #define _DEBUG_BEGIN(x)    Serial.begin(x);
  #define _DEBUG_PRINT(x)    Serial.print(x);
  #define _DEBUG_PRINTLN(x)  Serial.println(x);
  #define _DEBUG_WRITE(x)    Serial.write(x);
#else
  #define _DEBUG_BEGIN(x)
  #define _DEBUG_PRINT(x)
  #define _DEBUG_PRINTLN(x)
  #define _DEBUG_WRITE(x)
#endif

/* Server Kommunikation Botschaften */
#define SRVCOMM_PRINT

#ifdef SRVCOMM_PRINT
  #define _SRVCOMM_BEGIN(x)    Serial.begin(x);
  #define _SRVCOMM_PRINT(x)    Serial.print(x);
  #define _SRVCOMM_PRINTLN(x)  Serial.println(x);
  #define _SRVCOMM_WRITE(x)    Serial.write(x);
#else
  #define _SRVCOMM_BEGIN(x)
  #define _SRVCOMM_PRINT(x)
  #define _SRVCOMM_PRINTLN(x)
  #define _SRVCOMM_WRITE(x)
#endif

/* Telnet Server Port */
#define TELNET_PORT 23

/* Clients auf Server */
#define MAX_SRV_CLIENTS 2

/* LED für Client-Verbindung zum Server */
#define LED_SVR_HAS_CLNT D4

/* Pin für WS2812B Streifen */
#define WS2812B_PIN 15

/* Adresse für DS3231 */
#define DS3231_ADDRESS 0x68

/* AP Konfiguration */
IPAddress ap_ip(192,168,1,200);
IPAddress ap_gateway(192,168,1,254);
IPAddress ap_subnet(255,255,255,0);

/* WiFi Settings*/
const char *ap_ssid = "Wordclock";
const char *ap_password = "Wordclock";

#endif
