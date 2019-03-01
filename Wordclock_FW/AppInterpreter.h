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
    
    public://muss später in private geändert werden, sodass als Hauptfunktion nur readCommandCharFromApp verfügbar ist
        void _getCommandFromApp(char AppBefehl[11 ]);
        
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
        byte _convertArrayDataToByte(char ArrayData[11 ]);
        bool _convertArrayDataToBool(char ArrayData[11 ]);
        uint16_t _convertArrayDataToUint16(char ArrayData[11 ]);
        int x2i(char charArr[11]);
};
