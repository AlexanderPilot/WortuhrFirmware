/* Funktionen Setings */


/* Einbinden von Headerdateien */
#include "Settings.h"


/* Funktionen */
Settings::Settings()
{
      _Language = LANGUAGE_DE_DE;
      _CornersClockwise = true;
      _Brightness = 40;
}

byte Settings::getLanguage()
{
      return _Language;
}

void Settings::setLanguage(byte language)
{
      _Language = language;
}

byte Settings::getBrightness()
{
      return _Brightness;
}

void Settings::setBrightness(byte brightness)
{
      _Brightness = brightness;
}

boolean Settings::getCornersClockwise()
{
      return _CornersClockwise;
}

void Settings::setCornersClockwise(boolean CornersClockwise)
{
      _CornersClockwise = CornersClockwise;
}

void Settings::loadFromEEPROM()
{
      _Language = EEPROM.read(0);
      _CornersClockwise = EEPROM.read(1);
      _Brightness = EEPROM.read(2);
}

void Settings::saveToEEPROM()
{
      //EEPROM.update(0, _Language);
      //EEPROM.update(1, _CornersClockwise);
      //EEPROM.update(2, _Brightness);
}
