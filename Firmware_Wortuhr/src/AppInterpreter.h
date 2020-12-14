#pragma once

#include "Arduino.h"
#include "Configurations.h"
#include "Settings.h"

#define DEBUG_APPINTERPRETER 0

class AppInterpreter
{
public:
    //Konstruktor
    AppInterpreter();

    /****************************************
     * App Befehle einlesen
     ***************************************/
    void readCommandCharFromApp(char CommandChar);
    void setCommand(char AppBefehl[6]);
    uint8_t *getCommand();

private:
    /****************************************
     * Ansteuerbefehle aus der App
     ***************************************/
    void _CommSetColor(char AppBefehl[6]);
    void _CommSetBrightness(char AppBefehl[6]);
    void _CommSetTime(char AppBefehl[6]);
    void _CommSetMisc(char AppBefehl[6]);
    void _CommSetSSID(char AppBefehl[6], bool continueCommand);
    void _CommSetPW(char AppBefehl[6], bool continueCommand);

    /****************************************
         * Einstellungen vom Mikrocontroller lesen
         ***************************************/
    void _loadSettingsFromUC();
    byte _getLanguage();
    byte _getBrightnessPercent();
    pixel_t _getColor();
    byte _getFadeMode();
    byte _getCornerStartLed();
    bool _getCornersClockwise();
    byte _getStartPattern();
    uint16_t _getGmtTimeOffsetSec();

    /****************************************
         * Einstellungen auf Mikrocontroller setzen
         ***************************************/
    void _setLanguage(byte Language);
    void _setBrightnessPercent(byte Brightness);
    void _setColor(pixel_t color);
    void _setFadeMode(byte fadeMode);
    void _setCornerStartLed(byte CornerStartLed);
    void _setCornersClockwise(boolean Clockwise);
    void _setWifiSSID(String Ssid);
    void _setWifiPW(String Password);
    void _setStartPattern(byte StartPattern);
    void _setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec);

    /****************************************
         * Hilfsfunktionen für Datenkonvertierung
         ***************************************/
    byte _convertVarToByte(uint32_t ArrayData);
    bool _convertVarToBool(uint32_t ArrayData);
    uint16_t _convertVarToUint16(uint32_t ArrayData);
    uint8_t hexcharToUint8_t(char);
    uint8_t AppBefehl[6];
};
