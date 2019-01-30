/***************************************************************************
 * Header für die Einstellungsmöglichkeiten der Wortuhr
 *
 **************************************************************************/

/** Vermeidung Doppeldefinitionen **/
#pragma once

/** Einbinden von relevanten Bibliotheken **/
#include "Arduino.h"
#include "EEPROM.h"
#include "WS2812B.h"
#include "Configurations.h"

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
        const char *getWifiSSID();
        
        void setWifiPW(String Password);
        const char *getWifiPW();
        
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
        void loadAllFromEEPROM();
        byte loadLanguageFromEEPROM();
        byte loadBrightnessFromEEPROM();
        pixel_t loadColorFromEEPROM();
        byte loadFadeModeFromEEPROM();
        byte loadCornerStartLedFromEEPROM();
        bool loadCornerClockwiseFromEEPROM();
        String loadWifiSSIDFromEEPROM();
        String loadWifiPWFromEEPROM();
        byte loadStartpatternFromEEPROM();
        uint16_t loadGmtOffsetFromEEPROM();
        
        void writeLanguageToEEPROM(byte language);
        void writeBrightnessToEEPROM(byte brightness);
        void writeColorToEEPROM(pixel_t color);
        void writeFadeModeFromEEPROM(byte fademode);
        void writeCornerStartLedToEEPROM(byte cornerstartled);
        void writeCornerClockwiseToEEPROM(bool cornersclockwise);
        void writeWifiSSIDToEEPROM(String wifiSSID);
        void writeWifiPWToEEPROM(String wifiPW);
        void writeStartpatternToEEPROM(byte startpattern);
        void writeGmtOffsetToEEPROM(uint16_t gmtoffset);
        
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

