#pragma once

#include "Arduino.h"
#include "Configurations.h"
#include "Settings.h"

#define DEBUG_APPINTERPRETER 1

class AppInterpreter
{
public:
     //Konstruktor
     AppInterpreter();

     /****************************************
     * App Befehle einlesen
     ***************************************/
     void readCommandCharFromApp(char CommandChar);

private:
     /****************************************
     * Ansteuerbefehle aus der App
     ***************************************/
     void _CommSetColor(char *Farbe);
     void _CommSetBrightness(char *Helligkeit);
     void _CommSetTime(char *Uhrzeit);
     void _CommSetMisc(char *Misc);
     void _CommSetSSID(char *partialSSID, bool continueCommand);
     void _CommSetPW(char *partialPW, bool continueCommand);
     char _getDecryptedChar(char *c, uint8_t i);
     void _justSendTheFoundStringToSerial(char *p);

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
     void _setWifiSSID(char *Ssid);
     void _setWifiPW(char *Password);
     void _setStartPattern(byte StartPattern);
     void _setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec);

     /****************************************
      * Hilfsfunktionen für Datenkonvertierung
     ***************************************/
     byte _convertVarToByte(uint32_t ArrayData);
     bool _convertVarToBool(uint32_t ArrayData);
     uint16_t _convertVarToUint16(uint32_t ArrayData);
     uint8_t _hexcharToUint8_t(char);
};
