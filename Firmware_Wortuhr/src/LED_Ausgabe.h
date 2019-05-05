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
        void setPixelToMatrix(byte index);
        void setPixelToMatrix(byte index, byte red, byte green, byte blue);
        void setPixelToMatrix(byte xPos, byte yPos);
        void setPixelToMatrix(byte xPos, byte yPos, byte red, byte green, byte blue);
        void setMatrixToLEDs(word Matrix[12]);
        void clearLEDs();
        void showLEDs();
        
        /****************************************
         * LED-Übergang
         ***************************************/
        void MatrixToMatrixFade(word Matrix1[12],word Matrix2[12]);
        
        /****************************************
         * LED-Infos
         ***************************************/
        uint16_t getPixelCount();

         
    private:
        /****************************************
         * private LED Funktionen
         ***************************************/
        void _setPixel(byte xPos, byte yPos, byte red, byte green, byte blue);
        void _setPixel(byte xPos, byte yPos, pixel_t color);
        void _setPixel(byte LEDnum, byte red, byte green, byte blue);
        void _setPixel(byte LEDnum, pixel_t color);
        
        WS2812 *_strip;
};
