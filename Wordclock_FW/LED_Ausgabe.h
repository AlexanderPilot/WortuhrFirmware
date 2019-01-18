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
        void setPixelToMatrix(uint16_t index, byte red, byte green, byte blue);
        void setPixelToMatrix(byte xPos, byte yPos);
        void setPixelToMatrix(byte xPos, byte yPos, byte red, byte green, byte blue);
        void setMatrixToLEDs(word Matrix[11]);
        void clearLEDs();
        
        /****************************************
         * LED-Übergang
         ***************************************/
        void MatrixToMatrixFade(word Matrix1[11],word Matrix2[11]);
         
    private:
        /****************************************
         * private LED Funktionen
         ***************************************/
        void _setPixel(byte xPos, byte yPos, byte red, byte green, byte blue);
        void _setPixel(byte xPos, byte yPos, pixel_t color);
        void _setPixel(uint16_t LEDnum, byte red, byte green, byte blue);
        void _setPixel(uint16_t LEDnum, pixel_t color);
        
        WS2812 *_strip;
};
