/***************************************************************************
 * Funktionen für die Ausgabe von Bitmustern auf der LED-Matrix
 *
 **************************************************************************/


/* Einbinden von Headerdateien */
#include "LED_Ausgabe.h"

/* Funktionen */

LED_Ausgabe::LED_Ausgabe()
{
    _strip = new WS2812((gpio_num_t)LEDSTRIP_PIN,LED_NUM,0);
}

void LED_Ausgabe::LedStartUp(uint8_t Startpattern)
{
    _strip->startPattern(Startpattern);
    
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.print("Weitergabe des Startmusters Version ");
        Serial.print(Startpattern);
    }
}

void LED_Ausgabe::setPixelToMatrix(uint16_t index)
{
    _strip->setPixel(index);
    _strip->show();
    
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.print("LED Nummer ");
        Serial.println(index);
        Serial.println(" soll ausgegeben werden");
    }
}

void LED_Ausgabe::setPixelToMatrix(byte xPos, byte yPos, word Matix[11])
{
    
    
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("TBD ");
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.print("Pixel");
        Serial.print(" xpos: ");
        Serial.print(xPos);
        Serial.print(" ypos: ");
        Serial.print(yPos);
        Serial.println(" wird gesetzt");
    }
}

void LED_Ausgabe::setMatrixToLEDs(word Matrix[11])
{
    _strip->clear();
    //long color = _strip->Color(_BrightnessScaleColor(_Red), _BrightnessScaleColor(_Green), _BrightnessScaleColor(_Blue));

    for(byte yPos = 0; yPos <= 9; yPos++)         //Zeilen durchzählen
    {
          for(byte xPos = 5; xPos <= 15; xPos++)      //Spalten durchzählen
          {
                word BitMaske = 1 << xPos;
                if((Matrix[yPos] & BitMaske) == BitMaske)
                {
                      //Bit in der Matrix ist eine 1 -> LED an
                      //_setPixel(15-xPos, yPos, settings.getColor()); 
                }
          }
    }

    //Eck-LEDs umsetzen
    if((Matrix[10] & 0b1000000000000000) == 0b1000000000000000)
    {
        //_setPixel(110, color);
    }
      
    if((Matrix[10] & 0b0100000000000000) == 0b0100000000000000)
    {
        //_setPixel(111, color);
    }

    if((Matrix[10] & 0b0010000000000000) == 0b0010000000000000)
    {
        //_setPixel(112, color);
    }

    if((Matrix[10] & 0b0001000000000000) == 0b0001000000000000)
    {
        //_setPixel(113, color);
    }
    _strip->show();
}

void LED_Ausgabe::clearLEDs()
{
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp ");
        Serial.print("Alle LEDs sollen ausgeschaltet werden");
    }
    _strip->clear();
    _strip->show();
}
/*
byte LED_Ausgabe::_BrightnessScaleColor(byte colorPart)
{
    //Skaliert Prozentwert der Helligkeit auf Anteil der RGB Farbe
    return map(_Brightness, 0, 100, 0, colorPart);
}

void LED_Ausgabe::setPixelToMatrix(uint16_t index, word Matix[11])
{
    _strip->setPixelColor(index);
}

void LED_Ausgabe::setPixelToMatrix(byte xPos, byte yPos, word Matrix[11])
{
    Matrix[yPos] |= 0b1000000000000000 >> xPos;
}*/


/****************************************
 * private LED Funktionen
 ***************************************/
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
            //_strip->setPixelColor(LEDnum, color);
        }
        else
        {
            //_strip->setPixelColor(((LEDnum / 11) * 11) + 10 - (LEDnum % 11), color);
        }
    }
    else
    {
        //Eck-LEDs ansprechen
        switch(LEDnum)
        {
            case 110:
                //_strip->setPixelColor(111, color);
                break;
            case 111:
                //_strip->setPixelColor(112, color);
                break;
            case 112:
                //_strip->setPixelColor(113, color);
                break;
            case 113:
                //_strip->setPixelColor(110, color);
                break;
        }
    }
}
