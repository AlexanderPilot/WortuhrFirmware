/* Headerdatei fer Konfigurationen */

/* Vermeidung Doppeldefinitionen */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* Weitere Bibliotheken */
#include <ESP8266WiFi.h>

/* Serielle Geschwindigkeit */
#define SERIAL_SPEED 74880

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

//-------------------------------------------------------------------
// WiFi-Config!!!

/* Telnet Server Port */
#define TELNET_PORT 23

/* Clients auf Server */
#define MAX_SRV_CLIENTS 2

/* LED fuer Client-Verbindung zum Server */
//#define LED_SVR_HAS_CLNT D4

/* AP Konfiguration */
IPAddress serv_ip(192,168,1,200);
IPAddress serv_gateway(192,168,1,254);
IPAddress serv_subnet(255,255,255,0);

/* WiFi Settings*/
const char *ap_ssid = "Wordclock";
const char *ap_password = "";

/*AP Config */
IPAddress ap_ip(192,168,1,201);
IPAddress ap_gateway(192,168,1,254);
IPAddress ap_subnet(255,255,255,0);


//-------------------------------------------------------------------------
// Pixel Config

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define WS2812B_PIN    14

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      10


#endif
