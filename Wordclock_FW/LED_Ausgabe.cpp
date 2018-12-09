/* Funktionen LED_Ausgabe */


/* Einbinden von Headerdateien */
#include "LED_Ausgabe.h"

/* Funktionen */
/*
LED_Ausgabe::LED_Ausgabe(byte pinDataLED)
{
      _pinDataLED = pinDataLED;
      _strip = new Adafruit_NeoPixel(Pixel_Num, _pinDataLED, NEO_GRB + NEO_KHZ800);
      _strip->begin();
      _Red = 254;
      _Green = 255;
      _Blue = 253;
}
    
void LED_Ausgabe::init()
{
      Serial.println("LED init");
      setBrightness(20);
      clearLEDs();
}


void LED_Ausgabe::setMatrixToLEDs(word Matrix[11], boolean MinuteChange)
{
      if(MinuteChange = true)
      {
            _strip->clear();
            long color = _strip->Color(_BrightnessScaleColor(_Red), _BrightnessScaleColor(_Green), _BrightnessScaleColor(_Blue));
      
            for(byte yPos = 0; yPos <= 9; yPos++)         //Zeilen durchzählen
            {
                  for(byte xPos = 5; xPos <= 15; xPos++)      //Spalten durchzählen
                  {
                        word BitMaske = 1 << xPos;
                        if((Matrix[yPos] & BitMaske) == BitMaske)
                        {
                              //Bit in der Matrix ist eine 1 -> LED an
                              _setPixel(15-xPos, yPos, color); 
                        }
                  }
            }
            
            //Eck-LEDs umsetzen
            if((Matrix[10] & 0b1000000000000000) == 0b1000000000000000)
            {
                  _setPixel(110, color);
            }
              
            if((Matrix[10] & 0b0100000000000000) == 0b0100000000000000)
            {
                  _setPixel(111, color);
            }
            
            if((Matrix[10] & 0b0010000000000000) == 0b0010000000000000)
            {
                  _setPixel(112, color);
            }
            
            if((Matrix[10] & 0b0001000000000000) == 0b0001000000000000)
            {
                  _setPixel(113, color);
            }
            _strip->show();
      }
}

void LED_Ausgabe::setBrightness(byte BrightnessInPercent)
{
      _Brightness = BrightnessInPercent;
}

void LED_Ausgabe::clearLEDs()
{
      _strip->clear();
      _strip->show();
}

void LED_Ausgabe::_setPixel(byte xPos, byte yPos, long color)
{
      _setPixel(xPos + (yPos * 11), color);
}

void LED_Ausgabe::_setPixel(byte LEDnum, long color)
{
      //Keine Eck-LED angesprochen
      if(LEDnum < 110)
      {
            //jeder zweite LED Streifen wird "von hinten" angesprochen (Verkabelung)
            if((LEDnum / 11) % 2 == 0)
            {
            _strip->setPixelColor(LEDnum, color);
            }
            else {
            _strip->setPixelColor(((LEDnum / 11) * 11) + 10 - (LEDnum % 11), color);
            }
      }
      else
      {
            //Eck-LEDs ansprechen
            switch(LEDnum)
            {
                  case 110:
                        _strip->setPixelColor(111, color);  
                        break;
                  case 111:
                        _strip->setPixelColor(112, color);  
                        break;
                  case 112:
                        _strip->setPixelColor(113, color);  
                        break;
                  case 113:
                        _strip->setPixelColor(110, color);  
                        break;
            }
      }
}


byte LED_Ausgabe::_BrightnessScaleColor(byte colorPart)
{
      //Skaliert Prozentwert der Helligkeit auf Anteil der RGB Farbe
      return map(_Brightness, 0, 100, 0, colorPart);
}


void LED_Ausgabe::setPixelToMatrix(byte xPos, byte yPos, word Matrix[11])
{
      Matrix[yPos] |= 0b1000000000000000 >> xPos;
}

*/