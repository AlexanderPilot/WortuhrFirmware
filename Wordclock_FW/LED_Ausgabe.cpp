/***************************************************************************
 * Funktionen für die Ausgabe von Bitmustern auf der LED-Matrix
 *
 **************************************************************************/


/* Einbinden von Headerdateien */
#include "LED_Ausgabe.h"

/* Funktionen */

Settings _LEDsettings;

LED_Ausgabe::LED_Ausgabe()
{
    _strip = new WS2812((gpio_num_t)LEDSTRIP_PIN,LED_NUM,0);
}

void LED_Ausgabe::LedStartUp(uint8_t Startpattern)
{
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.print("Startmusters Version ");
    }
    
    switch(Startpattern)
    {
        case 0:
            if(DEBUG_LEDAUSGABE == 1)
            {
                Serial.print(Startpattern);
                Serial.println(" - alle LEDs ausschalten");
            }
            _strip->clear();
            _strip->show();
            break;

        case 1:
            if(DEBUG_LEDAUSGABE == 1)
            {
                Serial.print(Startpattern);
                Serial.println(" - alle LEDs rot");
            }
            _strip->setAllPixels(255,0,0);
            _strip->show();
            break;

        case 2:
            if(DEBUG_LEDAUSGABE == 1)
            {
                Serial.print(Startpattern);
                Serial.println(" - alle LEDs gruen");
            }
            _strip->setAllPixels(0,255,0);
            _strip->show();
            break;

        case 3:
            if(DEBUG_LEDAUSGABE == 1)
            {
                Serial.print(Startpattern);
                Serial.println(" - alle LEDs blau");
            }
            _strip->setAllPixels(0,0,255);
            _strip->show();
            break;

        case 4:
            if(DEBUG_LEDAUSGABE == 1)
            {
                Serial.print(Startpattern);
                Serial.println(" - LEDs nacheinander einschalten und dann nacheinander wieder ausschalten");
            }
            
            for (auto i=0; i<_strip->getPixelCount(); i++)
            {
                this->_setPixel(i, _LEDsettings.getColor());
                _strip->show();
                delay(STARTPATTERN_DELAY);
            }
            for (auto i=0; i<_strip->getPixelCount(); i++)
            {
                this->_setPixel(i,0,0,0);
                _strip->show();
                delay(STARTPATTERN_DELAY);
            }
            break;

        case 5:
            if(DEBUG_LEDAUSGABE == 1)
            {
                Serial.print(Startpattern);
                Serial.println(" - Zufallsmuster statisch");
            }
            for (auto i=0; i<_strip->getPixelCount(); i++)
            {
                this->_setPixel(i,random(255),random(255),random(255));
            }
            _strip->show();
            break;

        case 6:
            if(DEBUG_LEDAUSGABE == 1)
            {
                Serial.print(Startpattern);
                Serial.println(" - Zufallsmuster laufend");
            }
            for (auto i=0; i<_strip->getPixelCount(); i++)
            {
                this->_setPixel(i,random(255),random(255),random(255));
                _strip->show();
                delay(STARTPATTERN_DELAY);
            }
            break;
        case 7:
            if(DEBUG_LEDAUSGABE == 1)
            {
                Serial.print(Startpattern);
                Serial.println(" - TBD");
            }
            break;
        default:
            break;
    }
}


void LED_Ausgabe::setPixelToMatrix(uint16_t index)
{
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.print("LED Nummer ");
        Serial.print(index);
        Serial.println(" wird gesetzt");
    }
    
    this->_setPixel(index, _LEDsettings.getColor());
    _strip->show();
}

void LED_Ausgabe::setPixelToMatrix(uint16_t index, byte red, byte green, byte blue)
{
    this->_setPixel(index, red, green, blue);
    _strip->show();
}

void LED_Ausgabe::setPixelToMatrix(byte xPos, byte yPos, byte red, byte green, byte blue)
{
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.print("Pixel");
        Serial.print(" xpos: ");
        Serial.print(xPos);
        Serial.print(" ypos: ");
        Serial.print(yPos);
        Serial.println(" wird gesetzt");
    }
    
    this->_setPixel(xPos, yPos, red, green, blue);
    _strip->show();
}

void LED_Ausgabe::setPixelToMatrix(byte xPos, byte yPos)
{
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.print("Pixel");
        Serial.print(" xpos: ");
        Serial.print(xPos);
        Serial.print(" ypos: ");
        Serial.print(yPos);
        Serial.println(" wird gesetzt");
    }
    
    this->_setPixel(xPos, yPos, _LEDsettings.getColor());
    _strip->show();
}

void LED_Ausgabe::setMatrixToLEDs(word Matrix[11])
{
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.print("Berechnung der zu setzenden LEDs aus Matrix");
    }
    
    for(byte yPos = 0; yPos <= 9; yPos++)         //Zeilen durchzählen
    {
          for(byte xPos = 5; xPos <= 15; xPos++)      //Spalten durchzählen
          {
                word BitMaske = 1 << xPos;
                if((Matrix[yPos] & BitMaske) == BitMaske)
                {
                      //Bit in der Matrix ist eine 1 -> LED an
                      this->_setPixel(15-xPos, yPos, _LEDsettings.getColor()); 
                }
          }
    }

    //Eck-LEDs umsetzen
    if((Matrix[10] & 0b1000000000000000) == 0b1000000000000000)
    {
        this->_setPixel(110, _LEDsettings.getColor());
    }
      
    if((Matrix[10] & 0b0100000000000000) == 0b0100000000000000)
    {
        this->_setPixel(111, _LEDsettings.getColor());
    }

    if((Matrix[10] & 0b0010000000000000) == 0b0010000000000000)
    {
        this->_setPixel(112, _LEDsettings.getColor());
    }

    if((Matrix[10] & 0b0001000000000000) == 0b0001000000000000)
    {
        this->_setPixel(113, _LEDsettings.getColor());
    }
    
    _strip->show();
}

void LED_Ausgabe::clearLEDs()
{
    _strip->clear();
    _strip->show();
}

void LED_Ausgabe::MatrixToMatrixFade(word MatrixIst[11], word MatrixSoll[11])
{
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("TBD ");
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.println("Auswahl des LED Uebergangs (Wechsel, Ueberblenden, Fade");
    }
    byte FadeMode = 1;
    //Auslesen des Übergangsmodus aus den Settings
    switch(_LEDsettings.getFadeMode())
    switch(FadeMode)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
    
}

/****************************************
 * private LED Funktionen
 ***************************************/
void LED_Ausgabe::_setPixel(byte xPos, byte yPos, byte red, byte green, byte blue)
{
    pixel_t color;
    
    color.red = red;
    color.green = green;
    color.blue = blue;
    
    this->_setPixel(xPos + (yPos * 11), red, green, blue);
}
 
 void LED_Ausgabe::_setPixel(byte xPos, byte yPos, pixel_t color)
{
    this->_setPixel(xPos + (yPos * 11), color);
}

void LED_Ausgabe::_setPixel(uint16_t LEDnum, byte red, byte green, byte blue)
{
    pixel_t color;
    
    color.red = red;
    color.green = green;
    color.blue = blue;
    
    this->_setPixel(LEDnum, color);
}

void LED_Ausgabe::_setPixel(uint16_t LEDnum, pixel_t color)
{
    //Keine Eck-LED angesprochen
    if(LEDnum < 110)
    {
        //jeder zweite LED Streifen wird "von hinten" angesprochen (Verkabelung)
        if((LEDnum / 11) % 2 == 0)
        {
           _strip->setPixel(LEDnum, color);
        }
        else
        {
            _strip->setPixel(((LEDnum / 11) * 11) + 10 - (LEDnum % 11), color);
        }
    }
    else
    {
        //Eck-LEDs ansprechen
        switch(LEDnum)
        {
            case 110:
                _strip->setPixel(111, color);
                break;
            case 111:
                _strip->setPixel(112, color);
                break;
            case 112:
                _strip->setPixel(113, color);
                break;
            case 113:
                _strip->setPixel(110, color);
                break;
        }
    }
}
