/***************************************************************************
 * Header für die Konfiguarationen der Wortuhr
 *
 *
 **************************************************************************/


/* Vermeidung Doppeldefinitionen */
#pragma once

/***************************************************************************
 * Versionierung der Wortuhr
 * 
 * Version 1.0 wird vergeben,sobald die Wortuhr initial funktioniert
 * 
 * Versionshistorie:
 * v1.0 Erstinbetriebnahme der Wortuhr
 **************************************************************************/
#define VERSION "0.2"


/***************************************************************************
 * Einbinden von benötigten Bibliotheken
 **************************************************************************/
#include "WiFi.h"
#include "time.h"
#include "BluetoothSerial.h"
#include "Settings.h"
#include "AppInterpreter.h"

/***************************************************************************
 * Serielle Verbindung
 **************************************************************************/
#define SERIAL_SPEED 115200

/***************************************************************************
 * Debug Botschaften aktivieren/deaktivieren
 **************************************************************************/
#define DEBUG_PRINT

#ifdef DEBUG_PRINT
  #define _DEBUG_BEGIN(x)       Serial.begin(x);
  #define _DEBUG_PRINT(x)       Serial.print(x);
  #define _DEBUG_PRINTLN(x)     Serial.println(x);
  #define _DEBUG_WRITE(x)       Serial.write(x);
#else
  #define _DEBUG_BEGIN(x)
  #define _DEBUG_PRINT(x)
  #define _DEBUG_PRINTLN(x)
  #define _DEBUG_WRITE(x)
#endif

#define PRINT_SEPARATOR         "-------------------------"
#define PRINT_SEPARATOR_LONG    "--------------------------------------------------"


/***************************************************************************
 * WiFi Einstellungen
 **************************************************************************/
#define WIFI_MAX_TIME_CONNECTING    20  //max. Dauer, die gewartet wird bis Verbindungsaufbau abgebrochen wird

/***************************************************************************
 * NTP Server Einstellungen
 **************************************************************************/
#define NTP_SERVER_NAME     "pool.ntp.org"

/***************************************************************************
 * Bluetooth Einstellungen
 **************************************************************************/
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define BT_DEVICE_NAME    "Wordclock_AS"
#define MAX_BT_CLIENTS    1
#define BT_PASSWORD       "1234"

/***************************************************************************
 * App Interpreter Einstellungen
 **************************************************************************/
#define NUM_COMMAND_COUNT 11
#define NUM_SIGN_CATEGORY 3
#define START_SIGN  "+"
#define END_SIGN    "$"
#define SIGN_LANGUAGE "!"
#define SIGN_BRIGHTNESS "§"
#define SIGN_COLOR "$"
#define SIGN_FADEMODE "%"
#define SIGN_CORNERSTARTLED "&"
#define SIGN_CORNERSCLOCKWISE "/"
#define SIGN_STARTPATTERN "#"
#define SIGN_GMTOFFSET "?"

/***************************************************************************
 * I2C Einstellungen
 **************************************************************************/
/* Adresse fuer DS3231 */
#define DS3231_ADDRESS  0x68
#define SDA_PIN         21
#define SCL_PIN         22

/***************************************************************************
 * LED Einstellungen
 **************************************************************************/
#define LEDSTRIP_PIN        19
#define LED_NUM             114
#define STARTPATTERN_DELAY  50


/***************************************************************************
 * Timer Interrupt Einstellungen
 **************************************************************************/
//#define NTP_TIMER_VALUE_SEC 3600 //Aufruf der ISR und damit Synchronisation der Uhrzeit mit NTP Server alle Stunde
#define NTP_TIMER_VALUE_SEC 60 //für Testzwecke

/***************************************************************************
 * EEEPROM Einstellungen
 **************************************************************************/
#define EEPROM_SIZE 10
#define EEPROM_ADDRESS_LANGUAGE 1
#define EEPROM_ADDRESS_BRIGHTNESS 2
#define EEPROM_ADDRESS_COLOR_RED 3
#define EEPROM_ADDRESS_COLOR_GREEN 4
#define EEPROM_ADDRESS_COLOR_BLUE 5
#define EEPROM_ADDRESS_FADEMODE 6
#define EEPROM_ADDRESS_CORNERSTARTLED 7
#define EEPROM_ADDRESS_CORNERSCLOCKWISE 8
#define EEPROM_ADDRESS_WIFISSID 9
#define EEPROM_ADDRESS_WIFIPW 10
#define EEPROM_ADDRESS_STARTPATTERN 11
#define EEPROM_ADDRESS_GMTOFFSET 12
