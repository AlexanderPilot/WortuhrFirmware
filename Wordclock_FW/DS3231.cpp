/* Funktionen DS3231 */


/* Einbinden von Headerdateien */
#include "DS3231.h"



/* Funktionen */
DS3231::DS3231(int address)
{
      _address = address;
}

void DS3231::readTime()
{
      byte count = 0;
      
      //Beginne die I2C-Kommunikation mit dem DS3231 an der Adresse _address
      Wire.beginTransmission(_address);
      
      //Schreibe an die Anfangsadresse (ab welcher Adresse soll ausgelesen werden)
      Wire.write((uint8_t)0x00);
      
      //Auslesen der gueltigen Bits (Fehlerfall aussschliessen)
      count = Wire.requestFrom(_address, 7);
      
      if(count == 7)
      {
            //Alle 7 Bits sind vorhanden
            _Seconds   = BCDToDec(Wire.read() & 0x7F); //Maske um nur die notwendigen Bits auszulesen
            _Minutes   = BCDToDec(Wire.read()); 
            _Hours     = BCDToDec(Wire.read() & 0x3F);
            _DayOfWeek = BCDToDec(Wire.read());
            _Date      = BCDToDec(Wire.read());
            _Month     = BCDToDec(Wire.read());
            _Year      = BCDToDec(Wire.read());
      }
      else
      {
            //wenn Fehlerfall vorliegt
      }
  
      Wire.endTransmission();
}

/*ds3231.writeTime(struct structName);
{
      //Beginne die I2C-Kommunikation mit dem DS3231 an der Adresse _address
      Wire.beginTransmission(_address);

       //Schreibe an die Anfangsadresse (ab welcher Adresse soll ausgelesen werden)
      Wire.write((uint8_t)0x00);
      
      Wire.write(DecToBCD(structName.second));
      Wire.write(DecToBCD(structName.minute));
      Wire.write(DecToBCD(structName.hour));
      Wire.write(DecToBCD(structName.dayOfWeek));
      Wire.write(DecToBCD(structName.date));
      Wire.write(DecToBCD(structName.month));
      Wire.write(DecToBCD(structName.year));

      Wire.endTransmission();
}
*/

void DS3231::writeTime()
{
      //Beginne die I2C-Kommunikation mit dem DS3231 an der Adresse _address
      Wire.beginTransmission(_address);
      
       //Schreibe an die Anfangsadresse (ab welcher Adresse soll ausgelesen werden)
      Wire.write((uint8_t)0x00);
      
      Wire.write(DecToBCD(_Seconds));
      Wire.write(DecToBCD(_Minutes));
      Wire.write(DecToBCD(_Hours));
      Wire.write(DecToBCD(_DayOfWeek));
      Wire.write(DecToBCD(_Date));
      Wire.write(DecToBCD(_Month));
      Wire.write(DecToBCD(_Year));
      
      Wire.endTransmission();
}

void DS3231::printRTCTime(void)
{
      Serial.print("Uhrzeit: ");
      Serial.print(getHours());
      Serial.print(":");
      Serial.print(getMinutes());
      Serial.print(":");
      Serial.println(getSeconds());
}

void DS3231::setSeconds(byte Seconds)
{
      _Seconds = Seconds;
}

void DS3231::setMinutes(byte Minutes)
{
      _Minutes = Minutes;
}

void DS3231::setHours(byte Hours)
{
      _Hours = Hours;
}

void DS3231::setDayOfWeek(byte DayOfWeek)
{
      _DayOfWeek = DayOfWeek;
}

void DS3231::setDate(byte Date)
{
      _Date = Date;
}

void DS3231::setMonth(byte Month)
{
      _Month = Month;
}

void DS3231::setYear(byte Year)
{
      _Year = Year;
}

byte DS3231::getSeconds()
{
      return _Seconds;  
}

byte DS3231::getMinutes()
{
      return _Minutes;  
}

byte DS3231::getHours()
{
      return _Hours;  
}

byte DS3231::getDayOfWeek()
{
      return _DayOfWeek;  
}

byte DS3231::getDate()
{
      return _Date;  
}

byte DS3231::getMonth()
{
      return _Month;  
}

byte DS3231::getYear()
{
      return _Year; 
}

//Binary Code Decimal = Dateiformat der DS3231 Register

//Konvertiert decimal > bcd
byte DS3231::DecToBCD(byte value)
{
      return((value / 10 * 16) + (value % 10));
}

//Konvertiert bcd > decimal
byte DS3231::BCDToDec(byte value)
{
      return((value / 16 * 10) + (value % 16));
}


