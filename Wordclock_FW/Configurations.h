/* Headerdatei fuer Konfigurationen */

/* Vermeidung Doppeldefinitionen */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//---------------------------------------------------------------------------------
// Version

/* Versionierung */
#define VERSION "0.2" //v1.0 as soonas wordclock runs for firt time
/*
* History of Versions
* v1.0 initial commit of software
* 
*/

//---------------------------------------------------------------------------------
// Bibliotheken, abhängig vom Mikrocontroller
#ifdef ARDUINO_ESP32_DEV
#include "WiFi.h"
#include "WS2812B.h"
#include "DS3231.h"
#include "time.h"
#else
#include "ESP8266WiFi.h"
#endif

//---------------------------------------------------------------------------------
// Serielle Verbindung und Ausgabe

/* Serielle Geschwindigkeit */
#define SERIAL_SPEED 115200

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

#define PRINT_SEPARATOR "-------------------------"
#define PRINT_SMALLTAB "    "

//---------------------------------------------------------------------------------
// WiFi Settings

/* WiFi AP Settings*/
#define STA_SSID      "ASUS"
#define STA_PASSWORD  "Br8#Pojg56"
//#define STA_SSID        "Internet_MH"
//#define STA_PASSWORD    "WZ78AG27MGFF27DL"
//#define STA_SSID      "UPC68EE18B"
//#define STA_PASSWORD  "Tw11tYbolz@#"

uint8_t counterWiFiConnection = 0;


//---------------------------------------------------------------------------------
// NTP Settings
const char* NTP_SERVER_NAME = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

//---------------------------------------------------------------------------------
// Bluetooth Settings


#define MAX_BT_CLIENTS    1
#define BT_PASSWORD       "1234"



//---------------------------------------------------------------------------------
// I2C Settings

/* Adresse fuer DS3231 */
#define DS3231_ADDRESS 0x68
#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_FREQUENCY 400000


//---------------------------------------------------------------------------------
// LED Settings

#define LEDSTRIP_PIN               19
#define LED_NUM                        8
#define START_PATTERN          0








#endif

