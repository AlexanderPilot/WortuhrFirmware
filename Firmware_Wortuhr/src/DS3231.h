/***************************************************************************
 * Header für die Verbindung zur Echtzeituhr
 *
 **************************************************************************/

/* Vermeidung Doppeldefinitionen */
#pragma once

/* Weitere Bibliotheken */
#include <Arduino.h>
#include <Wire.h>

/* Klasse DS3231 */
class DS3231
{
    public:
        DS3231(int address);
        
        void readTime();
        void writeTime();
        void printRTCTime();
        
        void setSeconds(uint8_t Seconds);
        void setMinutes(uint8_t Minutes);
        void setHours(uint8_t Hours);
        void setDate(uint8_t Date);
        void setDayOfWeek(uint8_t DayOfWeek);
        void setMonth(uint8_t Month);
        void setYear(uint8_t Year);
        
        uint8_t getSeconds();
        uint8_t getMinutes();
        uint8_t getHours();
        uint8_t getDate();
        uint8_t getDayOfWeek();
        uint8_t getMonth();
        uint8_t getYear(); 
        
    private:
        int _address;

        uint8_t _Seconds;
        uint8_t _Minutes;
        uint8_t _Hours;
        uint8_t _Date;
        uint8_t _DayOfWeek;
        uint8_t _Month;
        uint8_t _Year;
        
        uint8_t DecToBCD(uint8_t value);
        uint8_t BCDToDec(uint8_t value);
};
