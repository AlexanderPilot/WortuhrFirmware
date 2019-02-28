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
        
        // Muss in der main hinter dem serverClients[i].read(); aufgerufen werden
        // Liesst die Zeichen und stellt die Helligkeit, Farbe und Weiteres ein
        bool comevatiation(char getChar);
    
        // Aufzurufen wenn comevatiation() ein true liefert
        // Setzt den Eingangsstring um
        void setUpCommand();
    
        // Loeschen eines Strings
        void deleteString(char a[]);
        
        
        
        
        
        /****************************************
         * App Befehle einlesen
         ***************************************/
        void getCommandFromApp(char AppBefehl[11]);
        void setCommandToApp(char AppBefehl[11]);
        
        /****************************************
         * Einstellungen vom Mikrocontroller lesen
         ***************************************/
        void loadSettingsFromUC();
        byte getLanguage();
        byte getBrightnessPercent();
        pixel_t getColor();
        byte getFadeMode();
        byte getCornerStartLed();
        boolen getCornersClockwise();
        byte getStartPattern();
        uint16_t getGmtTimeOffsetSec();
        
        /****************************************
         * Einstellungen auf Mikrocontroller setzen
         ***************************************/
        void setLanguage(byte Language);
        void setBrightnessPercent(byte Brightness);
        void setColor(pixel_t color);
        void setFadeMode(byte fadeMode);
        void setCornerStartLed(byte CornerStartLed);
        void setCornersClockwise(boolean Clockwise);
        void setWifiSSID(String Ssid);
        void setWifiPW(String Password);
        void setStartPattern(byte StartPattern);
        void setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec);
};
