/* Funktionen Zeitmaster */

/* Einbinden von Headerdateien */
#include "Zeitmaster.h"

/****************************************
 * Definition Objekte
 ***************************************/
NTPtime myNTP("de.pool.ntp.org");

/****************************************
 * Definition der static Variablen
 ***************************************/
strDateTime dateTime;

/***************************************************************************
 * Konstruktor der Klasse Zeitmaster
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
Zeitmaster::Zeitmaster()
{
    if ( ! myRTCDS3231.begin() )
    {
        while (1)
        {
            Serial.println("RTC ist nicht vorhanden oder funktioniert nicht richtig!\nAusführung des Programms wird beendet.");
            delay(3000);
        }
    }

    if (myRTCDS3231.lostPower())
    {
        Serial.println("RTC Batterie ist ausgefallen oder nicht vorhanden. Die Zeit wird auf Default (00:00:00) eingestellt. Bitte Batterie prüfen bzw. einbauen.\n");
        myRTCDS3231.adjust(DateTime(2020, 5, 4, 0, 0, 0));
    }
}

/***************************************************************************
 * Auslesen der gesamten Zeit und Datums Informationen
 * Übergabeparameter: kein, da die Zeit direkt vom DS3231 gelesen wird
 * Rückgabeparameter: gesamtes Zeit und Datums Struct
 **************************************************************************/
timedate_t Zeitmaster::getTimeDate()
{
    // TODO: Wochentag fehlt
    _TimeDate.year      =   myRTCDS3231.now().year();
    _TimeDate.month     =   myRTCDS3231.now().month();
    _TimeDate.date      =   myRTCDS3231.now().day();
    _TimeDate.hours     =   myRTCDS3231.now().hour();
    _TimeDate.minutes   =   myRTCDS3231.now().minute();
    _TimeDate.seconds   =   myRTCDS3231.now().second();

    return _TimeDate;
}

/***************************************************************************
 * Schreiben der gesamten Zeit und Datums Informationen
 * Übergabeparameter: gesamtes Zeit und Datums Struct
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setTimeDate(timedate_t TimeDate)
{
    myRTCDS3231.adjust( DateTime( TimeDate.year, TimeDate.month, TimeDate.date, TimeDate.hours, TimeDate.minutes, TimeDate.seconds ) );
}

/***************************************************************************
 * Schreiben der gesamten Zeit und Datums Informationen
 * Übergabeparameter: alle Zeit und Datums Informationen als einzelne Werte
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::setTimeDate(uint8_t Hours, uint8_t Minutes, uint8_t Seconds, uint8_t Date, uint8_t Month, uint8_t Year)
{
    myRTCDS3231.adjust( DateTime( Year, Month, Date, Hours, Minutes, Seconds ) );
}

/***************************************************************************
 * Auslesen der Sekunden im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Sekunden-Wert
 **************************************************************************/
uint8_t Zeitmaster::getSeconds()
{
    return myRTCDS3231.now().second();
}

/***************************************************************************
 * Auslesen der Minuten im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Minuten-Wert
 **************************************************************************/
uint8_t Zeitmaster::getMinutes()
{
    return myRTCDS3231.now().minute();
}

/***************************************************************************
 * Auslesen der Stunden im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Stunden-Wert
 **************************************************************************/
uint8_t Zeitmaster::getHours()
{
    return myRTCDS3231.now().hour();
}

/***************************************************************************
 * Auslesen des Tages im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Tages-Wert
 **************************************************************************/
uint8_t Zeitmaster::getDate()
{
    return myRTCDS3231.now().day();
}

/***************************************************************************
 * Auslesen des Wochentags im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Wochentags-Wert
 **************************************************************************/
uint8_t Zeitmaster::getDayOfWeek()
{
    //TODO: Auslesen des Wochentags
    return _TimeDate.dayOfWeek;
}

/***************************************************************************
 * Auslesen des Monats im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Monats-Wert
 **************************************************************************/
uint8_t Zeitmaster::getMonth()
{
    return myRTCDS3231.now().month();
}

/***************************************************************************
 * Auslesen des Jahres im Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: Jahres-Wert
 **************************************************************************/
uint8_t Zeitmaster::getYear()
{
    return myRTCDS3231.now().year();
}

/***************************************************************************
 * Schreiben der Informationen des Zeit und Datums Structs auf die serielle Ausgabe
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
void Zeitmaster::printZeitmasterTime(void)
{
    Serial.print("Uhrzeit: ");
    Serial.print(myRTCDS3231.now().hour());
    Serial.print(":");
    Serial.print(myRTCDS3231.now().minute());
    Serial.print(":");
    Serial.println(myRTCDS3231.now().second());
}


void Zeitmaster::printZeitmasterTimeMinuteByMinute(void)
{
    static uint8_t oldMinuteVal = 61;

    if( oldMinuteVal != myRTCDS3231.now().minute() )
    {
        oldMinuteVal = myRTCDS3231.now().minute();

        Serial.print("Uhrzeit: ");
        Serial.print(myRTCDS3231.now().hour());
        Serial.print(":");
        Serial.println(myRTCDS3231.now().minute());
    }   
}

bool Zeitmaster::timeTrigger()
{
    bool trigger = false;
    static uint8_t oldMinuteVal = 61;

    if( oldMinuteVal != myRTCDS3231.now().minute() )
    {
        oldMinuteVal = myRTCDS3231.now().minute();
        trigger = true;
    }
    return trigger;
}

void Zeitmaster::NtpTimeUpdate(float timezone, int daylightsaving)
{
    uint8_t retry = 0;
    bool time_new = false;
    
    while(retry <= NTP_MAX_TIME_CONNECTING && time_new == false)
    {
        dateTime = myNTP.getNTPtime(timezone, daylightsaving);
        if(dateTime.valid == true )
        {
            time_new = true;
            Serial.print("");
        }
        delay(10);
        retry++;
    }
    
    if(retry <= NTP_MAX_TIME_CONNECTING)
    {
        if(DEBUG_ZEITMASTER == 1)
        {
            Serial.print("Uhrzeit vom NTP Server: ");
            Serial.print(dateTime.hour);
            Serial.print(":");
            Serial.print(dateTime.minute);
            Serial.print(":");
            Serial.print(dateTime.second);
            Serial.print(" Datum vom NTP Server: ");
            Serial.print(dateTime.day);
            Serial.print(".");
            Serial.print(dateTime.month);
            Serial.print(".");
            Serial.println(dateTime.year);
        }
        retry = 0; //TODO: prüfen ob das hier benötigt wird
        //FIXME: korrekte Zeit vom NTP wird nicht aus RTC übernommen
        setTimeDate((uint8_t)dateTime.hour, (uint8_t)dateTime.minute, (uint8_t)dateTime.second, (uint8_t)dateTime.day, (uint8_t)dateTime.month, (uint8_t)dateTime.year);
    }
    delay(500);
}