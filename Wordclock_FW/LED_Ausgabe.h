/***************************************************************************
 * Header für die Ausgabe von Bitmustern auf der LED-Matrix
 *
 **************************************************************************/
/* Vermeidung Doppeldefinitionen */
#pragma once

#include "Arduino.h"
#include "Configurations.h"
#include "WS2812B.h"

#define DEBUG_LEDAUSGABE true

class LED_Ausgabe
{
    public:
        LED_Ausgabe();
        
        void init(byte StartPattern);
        
        void setMatrixToLEDs(word Matrix[11]);
        void setBrightness(byte BrightnessInPercent);
        
        void clearLEDs();
        void setPixelToMatrix(byte xPos, byte yPos, word Matix[11]);
        
    private:
        void _setPixel(byte xPos, byte yPos, long color);
        void _setPixel(byte LEDnum, long color);
        
        byte _BrightnessScaleColor(byte colorPart);
        
        WS2812 *_strip;
};
