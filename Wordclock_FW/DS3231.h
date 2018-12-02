/* Header für DS3231 */

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
            void setYear(uint16_t Year);
            
            uint8_t getSeconds();
            uint8_t getMinutes();
            uint8_t getHours();
            uint8_t getDate();
            uint8_t getDayOfWeek();
            uint8_t getMonth();
            uint16_t getYear(); 
            
      private:
            int _address;

            //struct für RTC Time
            //myTime *RtcTime;
            
            
            uint8_t _Seconds;
            uint8_t _Minutes;
            uint8_t _Hours;
            uint8_t _Date;
            uint8_t _DayOfWeek;
            uint8_t _Month;
            uint16_t _Year;
            
            uint16_t DecToBCD(uint16_t value);
            uint16_t BCDToDec(uint16_t value);
};
