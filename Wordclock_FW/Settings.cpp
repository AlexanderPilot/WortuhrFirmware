/***************************************************************************
 * Funktionen für die Einstellungsmöglichkeiten der Wortuhr
 *
 **************************************************************************/


/* Einbinden von Headerdateien */
#include "Settings.h"

/****************************************
 * Definition der static Variablen
 ***************************************/
byte Settings::_Language;
byte Settings::_Brightness;
pixel_t Settings::_Color;
byte Settings::_FadeMode;
byte Settings::_CornerStartLed;
boolean Settings::_CornersClockwise;
String Settings::_WifiSSID;
String Settings::_WifiPW;
byte Settings::_StartPattern;
uint16_t Settings::_GmtTimeOffsetSec;

/****************************************
 * Konstruktor mit Standardeinstellungen
 ***************************************/
Settings::Settings()
{
    //Konstruktor ist zu definieren, falls erforderlich
}

/****************************************
 * Sprache
 ***************************************/
void Settings::setLanguage(byte Language)
{
    _Language = Language;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Sprache");
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
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Helligkeit");
    }
}

byte Settings::getBrightnessPercent()
{
    return map(_Brightness, 0, 255, 0, 100);
}

/****************************************
 * LED Farbe
 ***************************************/
void Settings::setColor(pixel_t color)
{
    _Color = color;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Farbe - color");
    }
}

void Settings::setColor(byte red, byte green, byte blue)
{
    _Color.red = red;
    _Color.green = green;
    _Color.blue = blue;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Farbe - R,G,B");
    }
}

pixel_t Settings::getColor()
{
    return _Color;
}

/****************************************
 * LED Übergänge
 ***************************************/
void Settings::setFadeMode(byte fadeMode)
{
    _FadeMode = fadeMode;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe des FadeMode");
    }
}

byte Settings::getFadeMode()
{
    return _FadeMode;
}

/****************************************
 * Eck-LEDs
 ***************************************/
void Settings::setCornerStartLed(byte CornerStartLed)
{
    _CornerStartLed = CornerStartLed;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Start-Eck-LED");
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
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Drehrichtung der Eck-LEDs");
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
        Serial.print("Settings.cpp - ");
        Serial.print("WLAN Einstellungen");
        Serial.println(WifiSettingsAvailable ? "gueltig" : "ungueltig");
    }
    
    return WifiSettingsAvailable;
}

void Settings::setWifiSSID(String Ssid)
{
    _WifiSSID = Ssid;
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der WiFi SSID");
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
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe des WiFi Passworts");
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
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe des Startmusters");
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
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe des GMT Offsets");
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
