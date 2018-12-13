/***************************************************************************
 * Header für die Ausgabe von Bitmustern auf der LED-Matrix
 *
 **************************************************************************/
/* Vermeidung Doppeldefinitionen */
#pragma once

#include "Arduino.h"
#include "Configurations.h"
#include "WS2812B.h"

class LED_Ausgabe
{
      public:
	      LED_Ausgabe(byte pinDataLED);
            
            void init();
            void setMatrixToLEDs(word Matrix[11], boolean MinuteChange);
            void setBrightness(byte BrightnessInPercent);
            
            void clearLEDs();
            void setPixelToMatrix(byte xPos, byte yPos, word Matix[11]);
            
      private:
            unsigned int _Brightness;
            void _setPixel(byte xPos, byte yPos, long color);
            void _setPixel(byte LEDnum, long color);
            
            byte _Red;
            byte _Green;
            byte _Blue;
            
            byte _BrightnessScaleColor(byte colorPart);
            
            byte _pinDataLED;
            
            //Adafruit_NeoPixel *_strip;
                
};
