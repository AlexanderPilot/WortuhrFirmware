/***************************************************************************
 * Header für die Interpretation der Uhrzeit in Bitmuster für LED-Matrix (Rendern)
 *
 **************************************************************************/

/* Vermeidung Doppeldefinitionen */
#pragma once

#include "Arduino.h"
#include "Configurations.h"

#include "Woerter_DE.h"
#include "Woerter_EN.h"

#define LANGUAGE_DE_DE 0
#define LANGUAGE_DE_SW 1
#define LANGUAGE_DE_BA 2
#define LANGUAGE_DE_SA 3
#define LANGUAGE_CH    4
#define LANGUAGE_EN    5
#define LANGUAGE_FR    6
#define LANGUAGE_IT    7
#define LANGUAGE_NL    8
#define LANGUAGE_ES    9
#define LANGUAGE_COUNT 9

/* Klasse Renderer */
class Renderer
{
    public:
        Renderer();

        void setTime(byte Hours, byte Minutes, byte Language, word Matrix[11]);
        void setCorners(byte Minutes, word Matrix[12]);
        
        void cleanWordsMode(byte Language, word Matrix[11]);
        void setAllScreenOn(word Matrix[11]);
        void clearScreen(word Matrix[11]);
        
    private:
        void setHours(byte Hours, boolean glatt, byte Language, word Matrix[11]);
};
