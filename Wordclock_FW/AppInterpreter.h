#pragma once

#include "Settings.h"

class AppInterpreter
{
    public:
        //Konstruktor
        AppInterpreter();
        
        /****************************************
         * Setup d
         ***************************************/
        void setup();
    
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
        
        
        
        
        
        
        
        /****************************************
         * Einstellungen vom Mikrocontroller lesen
         ***************************************/
        
        
        /****************************************
         * Einstellungen auf Mikrocontroller setzen
         ***************************************/
        void setLanguage(byte Language);
        void setBrightnessPercent(byte Brightness);
        void setColor(pixel_t color);
        void setColor(byte red, byte green, byte blue);
        void setFadeMode(byte fadeMode);
        void setCornerStartLed(byte CornerStartLed);
        void setCornersClockwise(boolean Clockwise);
        void setWifiSSID(String Ssid);
        void setWifiPW(String Password);
        void setStartPattern(byte StartPattern);
        void setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec);
};
