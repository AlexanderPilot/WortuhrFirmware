/* Funktionen Time */

/* Einbinden von Headerdateien */
#include "time.h"

/* Funktionen */
time::time()
{

}

timedate_t time::getTimeDate()
{
    return _TimeDate;
}

void time::setTimeDate(timedate_t TimeDate)
{
    _TimeDate = TimeDate;
}

void time::setTimeDate(uint8_t Hours, uint8_t Minutes, uint8_t Seconds, uint8_t Date, uint8_t Month, uint8_t Year, uint8_t DayOfWeek)
{
    _TimeDate.hours = Hours;
    _TimeDate.minutes = Minutes;
    _TimeDate.seconds = Seconds;
    _TimeDate.date = Date;
    _TimeDate.dayOfWeek = DayOfWeek;
    _TimeDate.month = Month;
}

void time::setSeconds(uint8_t Seconds)
{
    _TimeDate.seconds = Seconds;
}

void time::setMinutes(uint8_t Minutes)
{
    _TimeDate.minutes = Minutes;
}

void time::setHours(uint8_t Hours)
{
    _TimeDate.hours = Hours;
}

void time::setDate(uint8_t Date)
{
    _TimeDate.date = Date;
}

void time::setDayOfWeek(uint8_t DayOfWeek)
{
    _TimeDate.dayOfWeek = DayOfWeek;
}

void time::setMonth(uint8_t Month)
{
    _TimeDate.month = Month;
}

void time::setYear(uint8_t Year)
{
    _TimeDate.year = Year;
}

uint8_t time::getSeconds()
{
    return _TimeDate.seconds;
}

uint8_t time::getMinutes()
{
    return _TimeDate.minutes;
}

uint8_t time::getHours()
{
    return _TimeDate.hours;
}

uint8_t time::getDate()
{
    return _TimeDate.date;
}

uint8_t time::getDayOfWeek()
{
    return _TimeDate.dayOfWeek;
}

uint8_t time::getMonth()
{
    return _TimeDate.month;
}

uint8_t time::getYear()
{
    return _TimeDate.year;
}
