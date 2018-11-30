/* Header für DS3231 */

/* Vermeidung Doppeldefinitionen */
#ifndef DS3231_H
#define DS3231_H

/* Weitere Bibliotheken */
#include <Arduino.h>
#include <Wire.h>

/* Klasse DS3231 */
class DS3231
{
      public:
            DS3231(int address);
            
            //myTime readTime();
            void readTime();
            //void writeTime(myTime rtcTime);
            void writeTime();
            void printRTCTime();
            
            void setSeconds(byte Seconds);
            void setMinutes(byte Minutes);
            void setHours(byte Hours);
            void setDate(byte Date);
            void setDayOfWeek(byte DayOfWeek);
            void setMonth(byte Month);
            void setYear(byte Year);
            
            byte getSeconds();
            byte getMinutes();
            byte getHours();
            byte getDate();
            byte getDayOfWeek();
            byte getMonth();
            byte getYear(); 
            
      private:
            int _address;

            //struct für RTC Time
            //myTime *RtcTime;
            
            
            byte _Seconds;
            byte _Minutes;
            byte _Hours;
	      byte _Date;
            byte _DayOfWeek;
            byte _Month;
            byte _Year;
            
            byte DecToBCD(byte value);
            byte BCDToDec(byte value);
};

#endif


