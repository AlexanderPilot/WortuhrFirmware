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
    _Language = LANGUAGE_DE_DE;
    _Brightness = 100;
    _Color.red = 255;
    _Color.green = 255;
    _Color.blue = 255;
    _FadeMode = 0;
    _CornerStartLed = 0;
    _CornersClockwise = 1;
    _WifiSSID = "ASUS";
    _WifiPW = "Br8#Pojg56";
    //_WifiSSID ="UPC68EE18B";
    //_WifiPW = "Tw11tYbolz@#";
    //_WifiSSID = "Internet_MH";
    //_WifiPW = "WZ78AG27MGFF27DL";
    _StartPattern = 4;
    _GmtTimeOffsetSec = 3600;
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

const char* Settings::getWifiSSID()
{
    const char *WifiSSID = _WifiSSID.c_str();
    return WifiSSID;
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

const char* Settings::getWifiPW()
{
    const char *WifiPW = _WifiPW.c_str();
    return WifiPW;
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

void Settings::loadAllFromEEPROM()
{
    _Language = this->loadLanguageFromEEPROM();
    _Brightness = this->loadBrightnessFromEEPROM(EEPROM_ADDR_BRIGHTNESS);
    _Color.red = this->loadFromEEPROM(EEPROM_ADDR_COLORRED);
    _Color.green = this->loadFromEEPROM(EEPROM_ADDR_COLORGREEN);
    _Color.blue = this->loadFromEEPROM(EEPROM_ADDR_COLORBLUE);
    _FadeMode = this->loadFromEEPROM(EEPROM_ADDR_FADEMODE);
    _CornerStartLed = this->loadFromEEPROM(EEPROM_ADDR_CORNERSTARTLED);
    _CornersClockwise = this->loadFromEEPROM(EEPROM_ADDR_CORNERCLOCKWISE);
    _WifiSSID = this->loadFromEEPROM(EEPROM_ADDR_WIFISSID);
    _WifiPW = this->loadFromEEPROM(EEPROM_ADDR_WIFIPW);
    _StartPattern = this->loadFromEEPROM(EEPROM_ADDR_STARTPATTERN);
    _GmtTimeOffsetSec = this->loadFromEEPROM(EEPROM_ADDR_GMTOFFSET);
}

byte Settings::loadLanguageFromEEPROM()
{
    byte EEPROMlanguage;
    EEPROMlanguage = EEPROM.readByte(EEPROM_ADDR_LANGUAGE);
        
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Sprache aus EEPROM: ");
        Serial.print(EEPROMlanguage);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_LANGUAGE);
        Serial.println(" gelesen");
    }
    return EEPROMlanguage;
}

byte Settings::loadBrightnessFromEEPROM()
{
    byte EEPROMbrightness;
    EEPROMbrightness = EEPROM.readByte(EEPROM_ADDR_BRIGHTNESS);
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Helligkeit aus EEPROM: ");
        Serial.print(EEPROMbrightness);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_BRIGHTNESS);
        Serial.println(" gelesen");
    }
    return EEPROMbrightness;
}

pixel_t Settings::loadColorFromEEPROM()
{
    pixel_t EEPROMcolor;
    EEPROMcolor.red = EEPROM.readByte(EEPROM_ADDR_COLORRED);
    EEPROMcolor.green = EEPROM.readByte(EEPROM_ADDR_COLORGREEN);
    EEPROMcolor.blue = EEPROM.readByte(EEPROM_ADDR_COLORBLUE);
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Farbe aus EEPROM: ");
        Serial.print("rot: ");
        Serial.print(EEPROMcolor.red);
        Serial.print("gruen: ");
        Serial.print(EEPROMcolor.green);
        Serial.print("blau: ");
        Serial.print(EEPROMcolor.blue);
        Serial.print(" wurde von den Adressen ");
        Serial.print("rot: ");
        Serial.print(EEPROM_ADDR_BRIGHTNESSRED);
        Serial.print("gruen: ");
        Serial.print(EEPROM_ADDR_BRIGHTNESSGREEN);
        Serial.print("blau: ");
        Serial.print(EEPROM_ADDR_BRIGHTNESSBLUE);
        Serial.println(" gelesen");
    }
    return EEPROMcolor;
}

byte Settings::loadFadeModeFromEEPROM()
{
    byte EEPROMfadeMode;
    EEPROMfadeMode = EEPROM.readByte(EEPROM_ADDR_FADEMODE);
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("FadeMode aus EEPROM: ");
        Serial.print(EEPROMfadeMode);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_FADEMODE);
        Serial.println(" gelesen");
    }
    return EEPROMfadeMode;
}

byte Settings::loadCornerStartLedFromEEPROM()
{
    byte EEPROMcornerStartLed;
    EEPROMcornerStartLed = EEPROM.readByte(EEPROM_ADDR_CORNERSTARTLED);
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Ecke der Start-LED aus EEPROM: ");
        Serial.print(EEPROMcornerStartLed);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_CORNERSTARTLED);
        Serial.println(" gelesen");
    }
    return EEPROMcornerStartLed;
}

bool Settings::loadCornerClockwiseFromEEPROM()
{
    bool EEPROMcornerClockwise;
    EEPROMcornerClockwise = EEPROM.readBool(EEPROM_ADDR_CORNERCLOCKWISE);
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Drehrichtung im Uhrzeugersinn aus EEPROM: ");
        Serial.print(EEPROMcornerClockwise);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_CORNERCLOCKWISE);
        Serial.println(" gelesen");
    }
    return EEPROMcornerClockwise;
}

String Settings::loadWifiSSIDFromEEPROM()
{
    String EEPROMwifiSSID;
    EEPROMwifiSSID = EEPROM.readString(EEPROM_ADDR_WIFISSID);
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("WifiSSID aus EEPROM: ");
        Serial.print(EEPROMwifiSSID);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_WIFISSID);
        Serial.println(" gelesen");
    }
    return EEPROMwifiSSID;
}

String Settings::loadWifiPWFromEEPROM()
{
    String EEPROMwifiPW;
    EEPROMwifiPW = EEPROM.readString(EEPROM_ADDR_WIFIPW);
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("WifiPW aus EEPROM: ");
        Serial.print(EEPROMwifiPW);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_WIFIPW);
        Serial.println(" gelesen");
    }
    return EEPROMwifiPW;
}

byte Settings::loadStartpatternFromEEPROM()
{
    byte EEPROMstartpattern;
    EEPROMstartpattern = EEPROM.readByte(EEPROM_ADDR_STARTPATTERN);
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Startmuster aus EEPROM: ");
        Serial.print(EEPROMstartpattern);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_STARTPATTERN);
        Serial.println(" gelesen");
    }
    return EEPROMstartpattern;
}

uint16_t Settings::loadGmtOffsetFromEEPROM()
{
    uint16_t EEPROMgmtOffset;
    EEPROMgmtOffset = EEPROM.readByte(EEPROM_ADDR_GMTOFFSET);
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("GMT Offset aus EEPROM: ");
        Serial.print(EEPROMgmtOffset);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_GMTOFFSET);
        Serial.println(" gelesen");
    }
    return EEPROMgmtOffset;
}


void Settings::saveToEEPROM(uint8_t address, uint8_t value)
{
    EEPROM.write(address, value);
    EEPROM.commit();
    
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Der Wert ");
        Serial.print(value);
        Serial.print(" wird auf Adresse ");
        Serial.print(address);
        Serial.print(" des EEPROM geschrieben");
    }
}
