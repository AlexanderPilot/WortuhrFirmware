/* Funktionen DS3231 */

/* Einbinden von Headerdateien */
#include "DS3231.h"



/* Funktionen */

/***************************************************************************
 * Konstruktor der Klasse DS3231
 * Übergabeparameter: I2C-Adresse des DS3231 Chips im hex-Format
 * Rückgabeparameter: kein
 **************************************************************************/
DS3231::DS3231(int address)
{
    _address = address;
}

/***************************************************************************
 * Liest die aktuelle Uhrzeit aus dem DS3231 Chip aus und speichert diese in einem Zeit und Datums Struct
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
timedate_t DS3231::readTime()
{
    uint8_t count = 0;
    timedate_t tmpTimeDate;

    //Beginne die I2C-Kommunikation mit dem DS3231 an der Adresse _address
    Wire.beginTransmission(_address);

    //Schreibe an die Anfangsadresse (ab welcher Adresse soll ausgelesen werden)
    Wire.write((uint8_t)0x00);

    //Auslesen der gueltigen Bits (Fehlerfall aussschliessen)
    count = Wire.requestFrom(_address, 7);

    if(count == 7)
    {
        //Alle 7 Bits sind vorhanden
        tmpTimeDate.seconds = BCDToDec(Wire.read() & 0x7F); //Maske um nur die notwendigen Bits auszulesen
        tmpTimeDate.minutes = BCDToDec(Wire.read()); 
        tmpTimeDate.hours = BCDToDec(Wire.read() & 0x3F);
        tmpTimeDate.dayOfWeek = BCDToDec(Wire.read());
        tmpTimeDate.date = BCDToDec(Wire.read());
        tmpTimeDate.month = BCDToDec(Wire.read());
        tmpTimeDate.year = BCDToDec(Wire.read());
    }
    else
    {
        //wenn Fehlerfall vorliegt
    }

    Wire.endTransmission();

    return tmpTimeDate;
}

/***************************************************************************
 * Schreibt die Uhrzeit aus dem Zeit und Datums Struct auf den DS3231 Chip 
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
void DS3231::writeTime(timedate_t tmpTimeDate)
{    
    //Beginne die I2C-Kommunikation mit dem DS3231 an der Adresse _address
    Wire.beginTransmission(_address);

    //Schreibe an die Anfangsadresse (ab welcher Adresse soll ausgelesen werden)
    Wire.write((uint8_t)0x00);
    Wire.write(DecToBCD(tmpTimeDate.seconds));
    Wire.write(DecToBCD(tmpTimeDate.minutes));
    Wire.write(DecToBCD(tmpTimeDate.hours));
    Wire.write(DecToBCD(tmpTimeDate.dayOfWeek));
    Wire.write(DecToBCD(tmpTimeDate.date));
    Wire.write(DecToBCD(tmpTimeDate.month));
    Wire.write(DecToBCD(tmpTimeDate.year));

    Wire.endTransmission();
}

void DS3231::writeTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t dayOfWeek, uint8_t date, uint8_t month, uint8_t year)
{
    //Beginne die I2C-Kommunikation mit dem DS3231 an der Adresse _address
    Wire.beginTransmission(_address);

    //Schreibe an die Anfangsadresse (ab welcher Adresse soll ausgelesen werden)
    Wire.write((uint8_t)0x00);
    Wire.write(DecToBCD(seconds));
    Wire.write(DecToBCD(minutes));
    Wire.write(DecToBCD(hours));
    Wire.write(DecToBCD(dayOfWeek));
    Wire.write(DecToBCD(date));
    Wire.write(DecToBCD(month));
    Wire.write(DecToBCD(year));

    Wire.endTransmission();
}

/***************************************************************************
 * Liest die Temperatur des DS3231 Chips
 * Übergabeparameter: kein
 * Rückgabeparameter: Temperatur des DS3231 Chips
 **************************************************************************/
uint8_t DS3231::getTemp()
{
    return 0;
}


//Binary Code Decimal = Dateiformat der DS3231 Register

/***************************************************************************
 * Konvertiert die Zeit und Datumsinformationen in BCD Format für die Beschreibung des DS3231 Chips
 * Übergabeparameter: Dezimalwert der in BCD umgerechnet werden soll
 * Rückgabeparameter: Umgerechneter Wert im BCD Format
 **************************************************************************/
uint8_t DS3231::DecToBCD(uint8_t value)
{
    return((value / 10 * 16) + (value % 10));
}

/***************************************************************************
 * Konvertiert die Zeit und Datumsinformationen in BCD Format des DS3231 Chips in Dezimalwerte um 
 * Übergabeparameter: BCD-Wert der in dezimal umgerechnet werden soll
 * Rückgabeparameter: Umgerechneter Wert im dezimal Format
 **************************************************************************/
uint8_t DS3231::BCDToDec(uint8_t value)
{
    return((value / 16 * 10) + (value % 16));
}

