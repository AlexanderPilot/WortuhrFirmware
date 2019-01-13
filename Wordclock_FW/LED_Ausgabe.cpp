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
            _strip->setPixel(1, _LEDsettings.getColor());
            _strip->show();
            /*for (auto i=0; i<this->pixelCount; i++)
            {
                this->pixels[i].red   = 255;
                this->pixels[i].green = 255;
                this->pixels[i].blue  = 255;
                this->show();
                delay(200);
            }
            for (auto i=0; i<this->pixelCount; i++)
            {
                this->pixels[i].red   = 0;
                this->pixels[i].green = 0;
                this->pixels[i].blue  = 0;
                this->show();
                delay(200);
            }*/
            _strip->show();
            break;

        case 5:
            if(DEBUG_LEDAUSGABE == 1)
            {
                Serial.print(Startpattern);
                Serial.println(" - Zufallsmuster");
            }
            /*for (auto i=0; i<this->pixelCount; i++)
            {
                this->pixels[i].red = random(255);
                this->pixels[i].green = random(255);
                this->pixels[i].blue = random(255);
            }*/
            _strip->show();
            break;

        case 6:
            if(DEBUG_LEDAUSGABE == 1)
            {
                Serial.print(Startpattern);
                Serial.println(" - tbd");
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
    
    this->_setPixel(index);
    _strip->show();
}

void LED_Ausgabe::setPixelToMatrix(byte xPos, byte yPos)
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
    
    this->_setPixel(xPos, yPos);
}

void LED_Ausgabe::setMatrixToLEDs(word Matrix[11])
{
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.print("Berechnung der zu setzenden LEDs aus Matrix");
    }
    
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
        //this->_setPixel(110);
    }
      
    if((Matrix[10] & 0b0100000000000000) == 0b0100000000000000)
    {
        //this->_setPixel(111);
    }

    if((Matrix[10] & 0b0010000000000000) == 0b0010000000000000)
    {
        //this->_setPixel(112);
    }

    if((Matrix[10] & 0b0001000000000000) == 0b0001000000000000)
    {
        //this->_setPixel(113);
    }
    
    _strip->show();
}

void LED_Ausgabe::clearLEDs()
{
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.println("Alle LEDs sollen ausgeschaltet werden");
    }
    
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
    //switch(settings.getFadeMode())
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
void LED_Ausgabe::_setPixel(byte xPos, byte yPos)
{
    _setPixel(xPos + (yPos * 11));
}

void LED_Ausgabe::_setPixel(uint16_t LEDnum)
{
    if(DEBUG_LEDAUSGABE == 1)
    {
        Serial.print("LED_Ausgabe.cpp - ");
        Serial.print("private Funktion wurde aufgerufen ");
        Serial.print("LED Nummer: ");
        Serial.println(LEDnum);
    }
    //Keine Eck-LED angesprochen
    if(LEDnum < 110)
    {
        //jeder zweite LED Streifen wird "von hinten" angesprochen (Verkabelung)
        if((LEDnum / 11) % 2 == 0)
        {
            //_strip->setPixel(LEDnum, color);
           _strip->setPixel(LEDnum, _LEDsettings.getColor());
        }
        else
        {
            //_strip->setPixelColor(((LEDnum / 11) * 11) + 10 - (LEDnum % 11), color);
            _strip->setPixel(((LEDnum / 11) * 11) + 10 - (LEDnum % 11), _LEDsettings.getColor());
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
