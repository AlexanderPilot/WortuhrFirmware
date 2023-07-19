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
#include "WS2812B.h"
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "BluetoothSerial.h"
#include "Renderer.h"
#include "Settings.h"
#include "LED_Ausgabe.h"
#include "AppInterpreter.h"
#include "Muster.h"
#include <Arduino.h>
#include "Zeitmaster.h"
#include "Preferences.h"


// Pin for LED control
#define LED_PIN 26

/***************************************************************************
 * Serielle Verbindung
 **************************************************************************/
#define SERIAL_SPEED 115200

/***************************************************************************
 * Debug Botschaften aktivieren/deaktivieren
 **************************************************************************/
#define DEBUG_PRINT 0

#if DEBUG_PRINT == 1
#define _DEBUG_BEGIN(x) Serial.begin(x);
#define _DEBUG_PRINT(x) Serial.print(x);
#define _DEBUG_PRINTLN(x) Serial.println(x);
#define _DEBUG_WRITE(x) Serial.write(x);
#else
#define _DEBUG_BEGIN(x)
#define _DEBUG_PRINT(x)
#define _DEBUG_PRINTLN(x)
#define _DEBUG_WRITE(x)
#endif

#define PRINT_SEPARATOR "-------------------------"
#define PRINT_SEPARATOR_LONG "--------------------------------------------------"

/***************************************************************************
 * Bluetooth Einstellungen
 **************************************************************************/
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

#define BT_DEVICE_NAME "Wordclock"
#define MAX_BT_CLIENTS 1
#define BT_PASSWORD "1234"

/***************************************************************************
 * WiFi Einstellungen
 **************************************************************************/
#define WIFI_HOSTNAME "wordclock"
#define SERVER_HOSTNAME "wordclock"
#define WIFI_MAX_TIME_CONNECTING 10 //max. Anzahl an 500ms delays, die gewartet wird bis Verbindungsaufbau abgebrochen wird

/***************************************************************************
 * NTP Server Einstellungen
 **************************************************************************/
#define NTP_SERVER_NAME "de.pool.ntp.org"
#define NTP_MAX_TIME_CONNECTING 50 //max. Anzahl an 10ms delays, die gewartet wird bis Verbindungsaufbau abgebrochen wird

/***************************************************************************
 * Spracheinstellungen
 **************************************************************************/
#define GERMAN 0

/***************************************************************************
 * App Interpreter Einstellungen
 **************************************************************************/
#define LENGTH_COMMAND_BUFFER 10
#define LENGTH_COMMAND_APP 6
#define POS_SIGN_STARTCOMMAND 9
#define POS_SIGN_ENDCOMMAND 1
#define POS_SIGN_END 0
#define POS_COMMAND_SIGN 8
#define SIGN_STARTCOMMAND 'X'
#define SIGN_ENDCOMMAND '$'
#define SIGN_END_CONTINUE '\t'
#define SIGN_END_ALL '\n'

#define SIGN_BRIGHTNESS 'H'
#define SIGN_COLOR 'F'
#define SIGN_CLOCK 'T'
#define SIGN_SSID 'S'
#define SIGN_PASSWORD 'P'

#define SIGN_LANGUAGE 'A'
#define SIGN_FADEMODE 'D'
#define SIGN_CORNERSTARTLED 'E'
#define SIGN_CORNERSCLOCKWISE 'F'
#define SIGN_STARTPATTERN 'G'
#define SIGN_GMTOFFSET 'H'
#define SIGN_CLOCKMODE 'M'
#define SIGN_RESET_TO_DEFAULT 'Q'
#define CONV_VERSION 2

#define MODE_CLOCK 0
#define MODE_GAMING 1

/***************************************************************************
 * I2C Einstellungen
 **************************************************************************/
/* Adresse fuer DS3231 */
#define DS3231_ADDRESS 0x68
#define SDA_PIN 21
#define SCL_PIN 22

/***************************************************************************
 * LED Einstellungen
 **************************************************************************/
#define LEDSTRIP_PIN 32
#define LED_NUM 144
#define STARTPATTERN_DELAY 50
#define NUM_COLUMN 12 //0-11 für die Zählweise
#define NUM_ROW 12    //0-11 für die Zählweise
#define LED_EINE_MIN 146
#define LED_ZWEI_MIN 147
#define LED_DREI_MIN 144
#define LED_VIER_MIN 145

/***************************************************************************
 * Timer Interrupt Einstellungen
 **************************************************************************/
//#define NTP_TIMER_VALUE_SEC 3600 //Aufruf der ISR und damit Synchronisation der Uhrzeit mit NTP Server alle Stunde
#define NTP_TIMER_VALUE_SEC 60 //TODO: für Testzweckegewählt, final größeren Wert
#define FACTOR_US_TO_S 1000000
