/***************************************************************************
 * Header für die Einstellungsmöglichkeiten der Wortuhr
 *
 **************************************************************************/

/** Vermeidung Doppeldefinitionen **/
#pragma once

/** Einbinden von relevanten Bibliotheken **/
#include "Arduino.h"
#include <EEPROM.h>
#include "Renderer.h"
#include "Configurations.h"

#define DEBUG_SETTINGS true

class Settings
{
    public:
        Settings();
        
        /****************************************
         * Sprache
         ***************************************/
        void setLanguage(byte Language);
        byte getLanguage();
        
        /****************************************
         * LED Grundeinstellungen
         ***************************************/
        void setBrightnessPercent(byte Brightness);
        byte getBrightnessPercent();
        
        //setColor();
        //getColor();
        
        /****************************************
         * Eck-LEDs
         ***************************************/
        void setCornerStartLed(byte CornerStartLed);
        byte getCornerStartLed();
        
        void setCornersClockwise(boolean Clockwise);
        boolean getCornersClockwise();
        
        /****************************************
         * WLAN SSID und Passwort
         ***************************************/
        bool getWifiSettingsAvailable();
        
        void setWifiSSID(String Ssid);
        String getWifiSSID();
        
        void setWifiPW(String Password);
        String getWifiPW();
        
        /****************************************
         * Starmuster
         ***************************************/
        void setStartPattern(byte StartPattern);
        byte getStartPattern();
        
        /****************************************
         * Offset zur GMT Zeit
         ***************************************/
        void setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec);
        uint16_t getGmtTimeOffsetSec();
        
        /****************************************
         * EEPROM Ansteuerung
         ***************************************/
        void loadFromEEPROM();
        void saveToEEPROM();
        
    private:
        /****************************************
         * interne Variablen
         ***************************************/
        byte _Language;
        byte _CornerStartLed;
        boolean _CornersClockwise;
        byte _Brightness;
        //byte _Color
        String _WifiSSID;
        String _WifiPW;
        byte _StartPattern;
        uint16_t _GmtTimeOffsetSec;
};

