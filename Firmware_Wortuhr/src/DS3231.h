/***************************************************************************
 * Header für die Verbindung zur Echtzeituhr
 *
 **************************************************************************/

/* Vermeidung Doppeldefinitionen */
#pragma once

/* Weitere Bibliotheken */
#include <Arduino.h>
#include <Wire.h>
#include "Zeitmaster.h"

/* Klasse DS3231 */
class DS3231
{
    public:
        DS3231(int address);
        
        timedate_t readTime();
        void writeTime(timedate_t tmpTimeDate);
        void writeTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t dayOfWeek, uint8_t date, uint8_t month, uint8_t year);
        uint8_t getTemp();
        
    private:
        int _address;

        uint8_t DecToBCD(uint8_t value);
        uint8_t BCDToDec(uint8_t value);
};
