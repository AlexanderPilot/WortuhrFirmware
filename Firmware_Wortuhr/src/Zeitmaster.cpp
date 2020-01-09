/* Funktionen Zeitmaster */

/* Einbinden von Headerdateien */
#include "Zeitmaster.h"

/* Funktionen */

/***************************************************************************
 * Konstruktor der Klasse Zeitmaster
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
Zeitmaster::Zeitmaster()
{
    _TimeDate.seconds = 10;
    _TimeDate.minutes = 5;
    _TimeDate.hours = 12;
}

/***************************************************************************
 * Auslesen der gesamten Zeit und Datums Informationen
 * Übergabeparameter: kein
 * Rückgabeparameter: gesamtes Zeit und Datums Struct
 **************************************************************************/
timedate_t Zeitmaster::getTimeDate()
{
    return _TimeDate;
}

/***************************************************************************
 * Schreiben der gesamten Zeit und Datums Informationen
 * Übergabeparameter: gesamtes Zeit und Datums Struct
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setTimeDate(timedate_t TimeDate)
{
    _TimeDate = TimeDate;
}

/***************************************************************************
 * Schreiben der gesamten Zeit und Datums Informationen
 * Übergabeparameter: alle Zeit und Datums Informationen als einzelne Werte
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setTimeDate(uint8_t Hours, uint8_t Minutes, uint8_t Seconds, uint8_t Date, uint8_t Month, uint8_t Year, uint8_t DayOfWeek)
{
    _TimeDate.hours = Hours;
    _TimeDate.minutes = Minutes;
    _TimeDate.seconds = Seconds;
    _TimeDate.date = Date;
    _TimeDate.dayOfWeek = DayOfWeek;
    _TimeDate.month = Month;
    _TimeDate.year = Year;
}

/***************************************************************************
 * Schreiben der Sekunden im Zeit und Datums Struct
 * Übergabeparameter: Sekunden-Wert
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setSeconds(uint8_t Seconds)
{
    _TimeDate.seconds = Seconds;
}

/***************************************************************************
 * Schreiben der Minuten im Zeit und Datums Struct
 * Übergabeparameter: Minuten-Wert
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setMinutes(uint8_t Minutes)
{
    _TimeDate.minutes = Minutes;
}

/***************************************************************************
 * Schreiben der Stunden im Zeit und Datums Struct
 * Übergabeparameter: Stunden-Wert
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setHours(uint8_t Hours)
{
    _TimeDate.hours = Hours;
}

/***************************************************************************
 * Schreiben des Tags im Zeit und Datums Struct
 * Übergabeparameter: Tag-Wert
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setDate(uint8_t Date)
{
    _TimeDate.date = Date;
}

/***************************************************************************
 * Schreiben des Wochentags im Zeit und Datums Struct
 * Übergabeparameter: Wochentags-Wert
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setDayOfWeek(uint8_t DayOfWeek)
{
    _TimeDate.dayOfWeek = DayOfWeek;
}

/***************************************************************************
 * Schreiben des Monats im Zeit und Datums Struct
 * Übergabeparameter: Monats-Wert
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setMonth(uint8_t Month)
{
    _TimeDate.month = Month;
}

/***************************************************************************
 * Schreiben des Jahrs im Zeit und Datums Struct
 * Übergabeparameter: Jahres-Wert
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setYear(uint8_t Year)
{
    _TimeDate.year = Year;
}

/***************************************************************************
 * Auslesen der Sekunden im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Sekunden-Wert
 **************************************************************************/
uint8_t Zeitmaster::getSeconds()
{
    return _TimeDate.seconds;
}

/***************************************************************************
 * Auslesen der Minuten im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Minuten-Wert
 **************************************************************************/
uint8_t Zeitmaster::getMinutes()
{
    return _TimeDate.minutes;
}

/***************************************************************************
 * Auslesen der Stunden im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Stunden-Wert
 **************************************************************************/
uint8_t Zeitmaster::getHours()
{
    return _TimeDate.hours;
}

/***************************************************************************
 * Auslesen des Tages im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Tages-Wert
 **************************************************************************/
uint8_t Zeitmaster::getDate()
{
    return _TimeDate.date;
}

/***************************************************************************
 * Auslesen des Wochentags im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Wochentags-Wert
 **************************************************************************/
uint8_t Zeitmaster::getDayOfWeek()
{
    return _TimeDate.dayOfWeek;
}

/***************************************************************************
 * Auslesen des Monats im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Monats-Wert
 **************************************************************************/
uint8_t Zeitmaster::getMonth()
{
    return _TimeDate.month;
}

/***************************************************************************
 * Auslesen des Jahres im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Jahres-Wert
 **************************************************************************/
uint8_t Zeitmaster::getYear()
{
    return _TimeDate.year;
}

/***************************************************************************
 * Schreiben der Informationen des Zeit und Datums Structs auf die serielle Ausgabe
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::printZeitmasterTime(void)
{
    Serial.print("Uhrzeit: ");
    Serial.print(_TimeDate.hours);
    Serial.print(":");
    Serial.print(_TimeDate.minutes);
    Serial.print(":");
    Serial.println(_TimeDate.seconds);
}