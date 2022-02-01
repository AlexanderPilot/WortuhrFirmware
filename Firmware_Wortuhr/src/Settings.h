/***************************************************************************
 * Header für die Einstellungsmöglichkeiten der Wortuhr
 *
 **************************************************************************/

/** Vermeidung Doppeldefinitionen **/
//#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

/** Einbinden von relevanten Bibliotheken **/
#include "Configurations.h"

#define DEBUG_SETTINGS 1

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
     * WLAN SSID und Passwort
     ***************************************/
     bool getWifiSettingsAvailable();

     void setWifiSSID(char *Ssid);
     char *getWifiSSID();

     void setWifiPW(char *Password);
     char *getWifiPW();

    /****************************************
     * Preferences
     ***************************************/
     bool allDataAvailable();
     void loadDataFromPreferences();
     void writeDataToPreferences();
     void clearPreferences();

    /****************************************
     * Initialisierungen
     ***************************************/
     bool startWifi();
     void startNtp();
     void startOTA();
     
     /****************************************
     * WiFi Funktionen (Reconnect + NTP + OTP)
     ***************************************/
     void WifiAutoReconnect();
     void NtpTimeUpdate();
     void handleOTA();

private:
     void loadLanguageFromPreferences();
     void writeLanguageToPreferences(byte language);
     
     void loadBrightnessFromPreferences();
     void writeBrightnessToPreferences(byte brightness);
     
     void loadColorFromPreferences();
     void writeColorToPreferences(pixel_t color);
     void writeColorToPreferences(byte red, byte green, byte blue);
     
     void loadFadeModeFromPreferences();
     void writeFadeModeToPreferences(byte fademode);
     
     void loadCornerStartLedFromPreferences();
     void writeCornerStartLedToPreferences(byte cornerstartled);
     
     void loadCornerClockwiseFromPreferences();
     void writeCornerClockwiseToPreferences(bool cornersclockwise);
     
     void loadStartpatternFromPreferences();
     void writeStartpatternToPreferences(byte startpattern);
     
     void loadGmtOffsetFromPreferences();
     void writeGmtOffsetToPreferences(uint16_t gmtoffset);
     
     void loadSsidFromPreferences();
     void writeSsidToPreferences(char* ssid);
     
     void loadPasswordFromPreferences();
     void writePasswordToPreferences(char* password);
     
     /****************************************
     * interne Variablen
     ***************************************/
     static byte _Language;
     static byte _Brightness;
     static pixel_t _Color;
     static byte _FadeMode;
     static byte _CornerStartLed;
     static boolean _CornersClockwise;
     static byte _StartPattern;
     static uint16_t _GmtTimeOffsetSec;
     static char _SSID_Array[32];
     static char _PW_Array[64];
};
#endif /* SETTINGS_H */