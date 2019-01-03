/***************************************************************************
 * Header für die Ausgabe von Bitmustern auf der LED-Matrix
 *
 **************************************************************************/
/* Vermeidung Doppeldefinitionen */
#pragma once

#include "Arduino.h"
#include "Configurations.h"
#include "WS2812B.h"
#include "Settings.h"

#define DEBUG_LEDAUSGABE 1

class LED_Ausgabe
{
    public:
        LED_Ausgabe();
        
        /****************************************
         * Startmuster der Wortuhr
         ***************************************/
        void LedStartUp(byte StartPattern);
        
        /****************************************
         * LED Funktionen
         ***************************************/
        void setPixelToMatrix(uint16_t index);
        void setPixelToMatrix(byte xPos, byte yPos, word Matix[11]);
        void setMatrixToLEDs(word Matrix[11]);
        void clearLEDs();
        
        /****************************************
         * LED-Übergang
         ***************************************/
         
    private:
        /****************************************
         * private LED Funktionen
         ***************************************/
        void _setPixel(byte xPos, byte yPos, long color);
        void _setPixel(byte LEDnum, long color);
        
        WS2812 *_strip;
};
