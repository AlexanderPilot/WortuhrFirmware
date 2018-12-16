/***************************************************************************
 * Header für die Einstellungsmöglichkeiten der Wortuhr
 *
 **************************************************************************/

/** Vermeidung Doppeldefinitionen **/
#pragma once

/** Einbinden von relevanten Bibliotheken **/
#include "Arduino.h"
#include <EEPROM.h>
#include "WS2812B.h"

#define DEBUG_SETTINGS 0

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
         * LED Helligkeit
         ***************************************/
        void setBrightnessPercent(byte Brightness);
        byte getBrightnessPercent();
        
        /****************************************
         * LED Farbe
         ***************************************/
        void setColor(pixel_t color);
        void setColor(byte red, byte green, byte blue);
        pixel_t getColor();
        
        /****************************************
         * LED Übergänge
         ***************************************/
        void setFadeMode(byte fadeMode);
        byte getFadeMode();
        
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
        static byte _Language;
        static byte _Brightness;
        static pixel_t _Color;
        static byte _FadeMode;
        static byte _CornerStartLed;
        static boolean _CornersClockwise;
        static String _WifiSSID;
        static String _WifiPW;
        static byte _StartPattern;
        static uint16_t _GmtTimeOffsetSec;
};

