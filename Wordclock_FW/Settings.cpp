/***************************************************************************
 * Funktionen für die Einstellungsmöglichkeiten der Wortuhr
 *
 **************************************************************************/


/* Einbinden von Headerdateien */
#include "Settings.h"


/****************************************
 * Konstruktor mit Standardeinstellungen
 ***************************************/
Settings::Settings()
{
    if(DEBUG_SETTINGS == 1)
    {
        Serial.println("Settings.cpp Konstruktor für Settings ist noch zu definieren, falls erforderlich");
    }
    //_Language = LANGUAGE_DE_DE;
    //_CornersClockwise = true;
    //_Brightness = 40;
}

/****************************************
 * Sprache
 ***************************************/
void Settings::setLanguage(byte Language)
{
    _Language = Language;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp Übergabe des Parameters Sprache ");
        Serial.print(Language);
        Serial.print(" an den internen Parameter ");
        Serial.println(_Language);
    }
}

byte Settings::getLanguage()
{
    return _Language;
}

/****************************************
 * LED Grundeinstellungen
 ***************************************/
void Settings::setBrightnessPercent(byte Brightness)
{
    _Brightness = map(Brightness, 0, 100, 0, 255);
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp Übergabe des Parameters Helligkeit ");
        Serial.print(Brightness);
        Serial.print(" an den internen Parameter ");
        Serial.println(_Brightness);
    }
}

byte Settings::getBrightnessPercent()
{
    return map(_Brightness, 0, 255, 0, 100);
}

void Settings::setColor(pixel_t color)
{
    _Color = color;
}

void Settings::setColor(byte red, byte green, byte blue)
{
    _Color.red = red;
    _Color.green = green;
    _Color.blue = blue;
}

pixel_t Settings::getColor()
{
    return _Color;
}



/****************************************
 * Eck-LEDs
 ***************************************/
void Settings::setCornerStartLed(byte CornerStartLed)
{
    _CornerStartLed = CornerStartLed;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp Übergabe des Parameters Start-Eck-LED ");
        Serial.print(CornerStartLed);
        Serial.print(" an den internen Parameter ");
        Serial.println(_CornerStartLed);
    }
}

byte Settings::getCornerStartLed()
{
    return _CornerStartLed;
}

void Settings::setCornersClockwise(boolean CornersClockwise)
{
    _CornersClockwise = CornersClockwise;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp Übergabe des Parameters Ecken im Uhrzeigersinn");
        Serial.print(CornersClockwise);
        Serial.print(" an den internen Parameter ");
        Serial.println(_CornersClockwise);
    }
}

boolean Settings::getCornersClockwise()
{
    return _CornersClockwise;
}

/****************************************
 * WLAN SSID und Passwort
 ***************************************/
bool Settings::getWifiSettingsAvailable()
{
    bool WifiSettingsAvailable = false;
    
    if(_WifiSSID.length() > 0 && _WifiPW.length() > 0)
    {
        WifiSettingsAvailable = true;
    }
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp WLAN Einstellungen ");
        Serial.println(WifiSettingsAvailable ? "gueltig" : "ungueltig");
    }
    
    return WifiSettingsAvailable;
}

void Settings::setWifiSSID(String Ssid)
{
    _WifiSSID = Ssid;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp Übergabe des Parameters WLAN SSID ");
        Serial.print(Ssid);
        Serial.print(" an den internen Parameter ");
        Serial.println(_WifiSSID);
    }

}

String Settings::getWifiSSID()
{
    return _WifiSSID;
}

void Settings::setWifiPW(String Password)
{
    _WifiPW = Password;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp Übergabe des Parameters WLAN Passwort ");
        Serial.print(Password);
        Serial.print(" an den internen Parameter ");
        Serial.println(_WifiPW);
    }
}

String Settings::getWifiPW()
{
    return _WifiPW;
}

/****************************************
 * Starmuster
 ***************************************/
void Settings::setStartPattern(byte StartPattern)
{
    _StartPattern = StartPattern;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp Übergabe des Parameters Startpattern ");
        Serial.print(StartPattern);
        Serial.print(" an den internen Parameter ");
        Serial.println(_StartPattern);
    }
}

byte Settings::getStartPattern()
{
    return _StartPattern;
}

/****************************************
 * Offset zur GMT Zeit
 ***************************************/

void Settings::setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec)
{
    _GmtTimeOffsetSec = GmtTimeOffsetSec;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp Übergabe des Parameters GMT Time Offset ");
        Serial.print(GmtTimeOffsetSec);
        Serial.print(" an den internen Parameter ");
        Serial.println(_GmtTimeOffsetSec);
    }
}

uint16_t Settings::getGmtTimeOffsetSec()
{
    return _GmtTimeOffsetSec;
}

/****************************************
 * EEPROM Ansteuerung
 ***************************************/

void Settings::loadFromEEPROM()
{
    Serial.println("Settings.cpp Laden vom EEPROM noch nicht implementiert (Settings.cpp)");
    //_Language = EEPROM.read(0);
    //_CornersClockwise = EEPROM.read(1);
    //_Brightness = EEPROM.read(2);
}


void Settings::saveToEEPROM()
{
    Serial.println("Settings.cpp Speichern auf EEPROM noch nicht implementiert (Settings.cpp)");
    //EEPROM.update(0, _Language);
    //EEPROM.update(1, _CornersClockwise);
    //EEPROM.update(2, _Brightness);
}
