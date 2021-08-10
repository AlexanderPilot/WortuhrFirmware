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
char *Settings::_WifiSSID;
char *Settings::_WifiPW;
byte Settings::_StartPattern;
uint16_t Settings::_GmtTimeOffsetSec;

/****************************************
 * Konstruktor mit Standardeinstellungen
 ***************************************/
Settings::Settings()
{
    _Language = LANGUAGE_DE_DE;
    _Brightness = 100;
    _Color.red = 30;
    _Color.green = 30;
    _Color.blue = 30;
    _FadeMode = 0;
    _CornerStartLed = 0;
    _CornersClockwise = 1;
    //_WifiSSID = {'0'};
    //_WifiPW = {'0'};
    //_WifiSSID = "OnLine";
    //_WifiPW = "Br8#Pojg56";
    //_WifiSSID ="UPC68EE18B";
    //_WifiPW = "Tw11tYbolz@#";
    //_WifiSSID = "Internet_MH";
    //_WifiPW = "WZ78AG27MGFF27DL";
    _StartPattern = 1;
    _GmtTimeOffsetSec = 3600;
}

/****************************************
 * Sprache
 ***************************************/
void Settings::setLanguage(byte Language)
{
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Sprache");
    }
    _Language = Language;
}

byte Settings::getLanguage()
{
    return _Language;
}

/****************************************
 * LED Helligkeit
 ***************************************/
void Settings::setBrightnessPercent(byte Brightness)
{
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Helligkeit");
    }
    _Brightness = map(Brightness, 0, 100, 0, 255);
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
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Farbe - color");
    }
    _Color = color;
}

void Settings::setColor(byte red, byte green, byte blue)
{
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Farbe - R,G,B");
    }
    _Color.red = red;
    _Color.green = green;
    _Color.blue = blue;
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
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe des FadeMode");
    }
    _FadeMode = fadeMode;
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
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Start-Eck-LED");
    }
    _CornerStartLed = CornerStartLed;
}

byte Settings::getCornerStartLed()
{
    return _CornerStartLed;
}

void Settings::setCornersClockwise(boolean CornersClockwise)
{
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der Drehrichtung der Eck-LEDs");
    }
    _CornersClockwise = CornersClockwise;
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

    if (sizeof(_WifiSSID) > 0 && sizeof(_WifiPW) > 0)
    {
        WifiSettingsAvailable = true;
    }

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("WLAN Einstellungen ");
        Serial.println(WifiSettingsAvailable ? "gueltig" : "ungueltig");
    }

    return WifiSettingsAvailable;
}

void Settings::setWifiSSID(char *Ssid)
{
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe der WiFi SSID");
    }
    _WifiSSID = Ssid;
}

char *Settings::getWifiSSID()
{
    return _WifiSSID;
}

void Settings::setWifiPW(char *Password)
{
    if (DEBUG_SETTINGS == 0)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe des WiFi Passworts ");
        Serial.print(Password);
        Serial.print(" und Speicherung in interve Variable ");
        Serial.println(_WifiPW);
    }
    _WifiPW = Password;
}

char *Settings::getWifiPW()
{
    return _WifiPW;
}

/****************************************
 * Starmuster
 ***************************************/
void Settings::setStartPattern(byte StartPattern)
{
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe des Startmusters");
    }
    _StartPattern = StartPattern;
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
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Übergabe des GMT Offsets");
    }
    _GmtTimeOffsetSec = GmtTimeOffsetSec;
}

uint16_t Settings::getGmtTimeOffsetSec()
{
    return _GmtTimeOffsetSec;
}

/****************************************
 * EEPROM Ansteuerung
 ***************************************/

/** EEPROM schreiben **/
bool Settings::checkEEPROMData()
{
    bool EEPROMdataOK = true;
    
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Daten aus dem EEPROM sind: ");
        Serial.println(EEPROMdataOK ? "gueltig" : "ungueltig");
    }
    Serial.println("Settings.cpp - checkEEPROMData Funktion muss noch implementiert werden, aktuell keine Pruefung der Daten");

    return EEPROMdataOK;
}

void Settings::loadAllFromEEPROM()
{
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.println("Alle Einstellungen vom EEPROM laden");
    }
    
    _Language = this->loadLanguageFromEEPROM();
    _Brightness = this->loadBrightnessFromEEPROM();
    _Color = this->loadColorFromEEPROM();
    _FadeMode = this->loadFadeModeFromEEPROM();
    _CornerStartLed = this->loadCornerStartLedFromEEPROM();
    _CornersClockwise = this->loadCornerClockwiseFromEEPROM();
    //_WifiSSID = this->loadWifiSSIDFromEEPROM();
    //_WifiPW = this->loadWifiPWFromEEPROM();
    _StartPattern = this->loadStartpatternFromEEPROM();
    _GmtTimeOffsetSec = this->loadGmtOffsetFromEEPROM();
}

byte Settings::loadLanguageFromEEPROM()
{
    byte EEPROMlanguage;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Sprache aus EEPROM: ");
        Serial.print(EEPROMlanguage);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_LANGUAGE);
        Serial.println(" gelesen");
    }
    EEPROMlanguage = EEPROM.readByte(EEPROM_ADDR_LANGUAGE);
    return EEPROMlanguage;
}

byte Settings::loadBrightnessFromEEPROM()
{
    byte EEPROMbrightness;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Helligkeit aus EEPROM: ");
        Serial.print(EEPROMbrightness);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_BRIGHTNESS);
        Serial.println(" gelesen");
    }
    EEPROMbrightness = EEPROM.readByte(EEPROM_ADDR_BRIGHTNESS);
    return EEPROMbrightness;
}

pixel_t Settings::loadColorFromEEPROM()
{
    pixel_t EEPROMcolor;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Farbe aus EEPROM: ");
        Serial.print("rot: ");
        Serial.print(EEPROMcolor.red);
        Serial.print(" gruen: ");
        Serial.print(EEPROMcolor.green);
        Serial.print(" blau: ");
        Serial.print(EEPROMcolor.blue);
        Serial.print(" wurde von den Adressen ");
        Serial.print("rot: ");
        Serial.print(EEPROM_ADDR_COLORRED);
        Serial.print(" gruen: ");
        Serial.print(EEPROM_ADDR_COLORGREEN);
        Serial.print(" blau: ");
        Serial.print(EEPROM_ADDR_COLORBLUE);
        Serial.println(" gelesen");
    }
    EEPROMcolor.red = EEPROM.readByte(EEPROM_ADDR_COLORRED);
    EEPROMcolor.green = EEPROM.readByte(EEPROM_ADDR_COLORGREEN);
    EEPROMcolor.blue = EEPROM.readByte(EEPROM_ADDR_COLORBLUE);
    return EEPROMcolor;
}

byte Settings::loadFadeModeFromEEPROM()
{
    byte EEPROMfadeMode;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("FadeMode aus EEPROM: ");
        Serial.print(EEPROMfadeMode);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_FADEMODE);
        Serial.println(" gelesen");
    }
    EEPROMfadeMode = EEPROM.readByte(EEPROM_ADDR_FADEMODE);
    return EEPROMfadeMode;
}

byte Settings::loadCornerStartLedFromEEPROM()
{
    byte EEPROMcornerStartLed;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Ecke der Start-LED aus EEPROM: ");
        Serial.print(EEPROMcornerStartLed);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_CORNERSTARTLED);
        Serial.println(" gelesen");
    }
    EEPROMcornerStartLed = EEPROM.readByte(EEPROM_ADDR_CORNERSTARTLED);
    return EEPROMcornerStartLed;
}

bool Settings::loadCornerClockwiseFromEEPROM()
{
    bool EEPROMcornerClockwise;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Drehrichtung im Uhrzeigersinn aus EEPROM: ");
        Serial.print(EEPROMcornerClockwise);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_CORNERCLOCKWISE);
        Serial.println(" gelesen");
    }
    EEPROMcornerClockwise = EEPROM.readBool(EEPROM_ADDR_CORNERCLOCKWISE);
    return EEPROMcornerClockwise;
}
/*
char* Settings::loadWifiSSIDFromEEPROM()
{
    char* EEPROMwifiSSID;
    EEPROMwifiSSID = EEPROM.readString(EEPROM_ADDR_WIFISSID);

    if (DEBUG_SETTINGS == 1)
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

    if (DEBUG_SETTINGS == 1)
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
*/
byte Settings::loadStartpatternFromEEPROM()
{
    byte EEPROMstartpattern;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Startmuster aus EEPROM: ");
        Serial.print(EEPROMstartpattern);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_STARTPATTERN);
        Serial.println(" gelesen");
    }
    EEPROMstartpattern = EEPROM.readByte(EEPROM_ADDR_STARTPATTERN);
    return EEPROMstartpattern;
}

uint16_t Settings::loadGmtOffsetFromEEPROM()
{
    uint16_t EEPROMgmtOffset;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("GMT Offset aus EEPROM: ");
        Serial.print(EEPROMgmtOffset);
        Serial.print(" wurde von Adresse ");
        Serial.print(EEPROM_ADDR_GMTOFFSET);
        Serial.println(" gelesen");
    }
    EEPROMgmtOffset = EEPROM.readByte(EEPROM_ADDR_GMTOFFSET);
    return EEPROMgmtOffset;
}

/** EEPROM schreiben **/
void Settings::writeLanguageToEEPROM(byte language)
{
    size_t EEPROMstorage;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Sprache: ");
        Serial.print(language);
        Serial.print(" wird auf EEPROM Adresse ");
        Serial.print(EEPROM_ADDR_LANGUAGE);
        Serial.println(" geschrieben");
    }
    EEPROMstorage = EEPROM.writeByte(EEPROM_ADDR_LANGUAGE, language);
    EEPROM.commit();
}

void Settings::writeBrightnessToEEPROM(byte brightness)
{
    size_t EEPROMstorage;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Helligkeit: ");
        Serial.print(brightness);
        Serial.print(" wird auf EEPROM Adresse ");
        Serial.print(EEPROM_ADDR_BRIGHTNESS);
        Serial.println(" geschrieben");
    }
    EEPROMstorage = EEPROM.writeByte(EEPROM_ADDR_BRIGHTNESS, brightness);
    EEPROM.commit();
}

void Settings::writeColorToEEPROM(pixel_t color)
{
    size_t EEPROMstorage_red;
    size_t EEPROMstorage_green;
    size_t EEPROMstorage_blue;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Farbe: ");
        Serial.print("rot: ");
        Serial.print(color.red);
        Serial.print("gruen: ");
        Serial.print(color.green);
        Serial.print("blau: ");
        Serial.print(color.blue);
        Serial.print(" wurde auf die Adressen ");
        Serial.print("rot: ");
        Serial.print(EEPROM_ADDR_COLORRED);
        Serial.print("gruen: ");
        Serial.print(EEPROM_ADDR_COLORGREEN);
        Serial.print("blau: ");
        Serial.print(EEPROM_ADDR_COLORBLUE);
        Serial.println(" geschrieben");
    }
    EEPROMstorage_red = EEPROM.writeByte(EEPROM_ADDR_COLORRED, color.red);
    EEPROMstorage_green = EEPROM.writeByte(EEPROM_ADDR_COLORGREEN, color.green);
    EEPROMstorage_blue = EEPROM.writeByte(EEPROM_ADDR_COLORBLUE, color.blue);
    EEPROM.commit();
}

void Settings::writeFadeModeFromEEPROM(byte fademode)
{
    size_t EEPROMstorage;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("FadeMode: ");
        Serial.print(fademode);
        Serial.print(" wird auf EEPROM Adresse ");
        Serial.print(EEPROM_ADDR_FADEMODE);
        Serial.println(" geschrieben");
    }
    EEPROMstorage = EEPROM.writeByte(EEPROM_ADDR_FADEMODE, fademode);
    EEPROM.commit();
}

void Settings::writeCornerStartLedToEEPROM(byte cornerstartled)
{
    size_t EEPROMstorage;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Ecke der Start-LED: ");
        Serial.print(cornerstartled);
        Serial.print(" wird auf EEPROM Adresse ");
        Serial.print(EEPROM_ADDR_CORNERSTARTLED);
        Serial.println(" geschrieben");
    }
    EEPROMstorage = EEPROM.writeByte(EEPROM_ADDR_CORNERSTARTLED, cornerstartled);
    EEPROM.commit();
}

void Settings::writeCornerClockwiseToEEPROM(bool cornersclockwise)
{
    size_t EEPROMstorage;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Drehrichtung im Uhrzeigersinn: ");
        Serial.print(cornersclockwise);
        Serial.print(" wird auf EEPROM Adresse ");
        Serial.print(EEPROM_ADDR_CORNERCLOCKWISE);
        Serial.println(" geschrieben");
    }
    EEPROMstorage = EEPROM.writeBool(EEPROM_ADDR_CORNERCLOCKWISE, cornersclockwise);
    EEPROM.commit();
}
/*
void Settings::writeWifiSSIDToEEPROM(String wifiSSID)
{
    size_t EEPROMstorage;
    EEPROMstorage = EEPROM.writeString(EEPROM_ADDR_WIFISSID, wifiSSID);
    EEPROM.commit();

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("WifiSSID: ");
        Serial.print(wifiSSID);
        Serial.print(" wird auf EEPROM Adresse ");
        Serial.print(EEPROM_ADDR_WIFISSID);
        Serial.println(" geschrieben");
    }
}

void Settings::writeWifiPWToEEPROM(String wifiPW)
{
    size_t EEPROMstorage;
    EEPROMstorage = EEPROM.writeString(EEPROM_ADDR_WIFIPW, wifiPW);
    EEPROM.commit();

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("WifiPW: ");
        Serial.print(wifiPW);
        Serial.print(" wird auf EEPROM Adresse ");
        Serial.print(EEPROM_ADDR_WIFIPW);
        Serial.println(" geschrieben");
    }
}
*/
void Settings::writeStartpatternToEEPROM(byte startpattern)
{
    size_t EEPROMstorage;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Starpattern: ");
        Serial.print(startpattern);
        Serial.print(" wird auf EEPROM Adresse ");
        Serial.print(EEPROM_ADDR_STARTPATTERN);
        Serial.println(" geschrieben");
    }
    EEPROMstorage = EEPROM.writeByte(EEPROM_ADDR_STARTPATTERN, startpattern);
    EEPROM.commit();
}

void Settings::writeGmtOffsetToEEPROM(uint16_t gmtoffset)
{
    size_t EEPROMstorage;

    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("GMT Offset: ");
        Serial.print(gmtoffset);
        Serial.print(" wird auf EEPROM Adresse ");
        Serial.print(EEPROM_ADDR_GMTOFFSET);
        Serial.println(" geschrieben");
    }
    EEPROMstorage = EEPROM.writeUShort(EEPROM_ADDR_GMTOFFSET, gmtoffset);
    EEPROM.commit();
}
