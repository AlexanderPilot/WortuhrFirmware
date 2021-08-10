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
byte Settings::_StartPattern;
uint16_t Settings::_GmtTimeOffsetSec;

/****************************************
 * Konstruktor mit Standardeinstellungen
 ***************************************/
Settings::Settings()
{
    _Language = LANGUAGE_DE_DE;
    _Brightness = 100;
    _Color.red = 0;
    _Color.green = 0;
    _Color.blue = 30;
    _FadeMode = 0;
    _CornerStartLed = 0;
    _CornersClockwise = 1;
    _StartPattern = 1;
    _GmtTimeOffsetSec = 3600;
    //FIXME: mit der Zeile unterhalb führt es zu CPU Resets
    //EEPROM.begin(EEPROM_SIZE);
}

/****************************************
 * Sprache
 ***************************************/
void Settings::setLanguage(byte Language)
{
    if(_Language != Language)
    {
        _Language = Language;
        writeLanguageToEEPROM(_Language);
    }
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
    if(_Brightness != map(Brightness, 0, 100, 0, 255))
    {
        _Brightness = map(Brightness, 0, 100, 0, 255);
        writeBrightnessToEEPROM(_Brightness);
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
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.println("Übergabe der Farbe - color");
    }
    if((_Color.red != color.red) || (_Color.green != color.green) || (_Color.blue != color.blue))
    {
        //_Color = color;
        writeColorToEEPROM(_Color);
        _Color = color;
        //TODO: Umstellun auf direktes Lesen vom EEPROM
        //_Color = loadColorFromEEPROM();
    }
}

void Settings::setColor(byte red, byte green, byte blue)
{
    if((_Color.red != red) || (_Color.green != green) || (_Color.blue != blue))
    {
        _Color.red = red;
        _Color.green = green;
        _Color.blue = blue;
        writeColorToEEPROM(_Color);
    }
}

pixel_t Settings::getColor()
{
    return _Color;
}

/****************************************
 * LED Übergänge
 ***************************************/
void Settings::setFadeMode(byte FadeMode)
{
    if(_FadeMode != FadeMode)
    {
        _FadeMode = FadeMode;
        writeFadeModeToEEPROM(_FadeMode);
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
    if(_CornerStartLed != CornerStartLed)
    {
        _CornerStartLed = CornerStartLed;
        writeCornerStartLedToEEPROM(_CornerStartLed);
    }
    
}

byte Settings::getCornerStartLed()
{
    return _CornerStartLed;
}

void Settings::setCornersClockwise(boolean CornersClockwise)
{
    if(_CornersClockwise != CornersClockwise)
    {
        _CornersClockwise = CornersClockwise;
        writeCornerClockwiseToEEPROM(_CornersClockwise);
    }
}

boolean Settings::getCornersClockwise()
{
    return _CornersClockwise;
}

/****************************************
 * Starmuster
 ***************************************/
void Settings::setStartPattern(byte StartPattern)
{
    if(_StartPattern != StartPattern)
    {
        _StartPattern = StartPattern;
        writeStartpatternToEEPROM(_StartPattern);
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
    if(_GmtTimeOffsetSec != GmtTimeOffsetSec)
    {
        _GmtTimeOffsetSec = GmtTimeOffsetSec;
        writeGmtOffsetToEEPROM(_GmtTimeOffsetSec);
    }
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
    //TODO: Implementieren der Funktion
    return EEPROMdataOK;
}

void Settings::loadAllFromEEPROM()
{
    _Language = this->loadLanguageFromEEPROM();
    _Brightness = this->loadBrightnessFromEEPROM();
    _Color = this->loadColorFromEEPROM();
    _FadeMode = this->loadFadeModeFromEEPROM();
    _CornerStartLed = this->loadCornerStartLedFromEEPROM();
    _CornersClockwise = this->loadCornerClockwiseFromEEPROM();
    _StartPattern = this->loadStartpatternFromEEPROM();
    _GmtTimeOffsetSec = this->loadGmtOffsetFromEEPROM();
}

/***************************************************************************
 * Auslesen der Sprache aus EEPROM
 * Übergabeparameter: kein, da die Sprache direkt vom EEPROM gelesen wird
 * Rückgabeparameter: Sprache die verwendet werden soll
 **************************************************************************/
byte Settings::loadLanguageFromEEPROM()
{
    byte EEPROMlanguage;
    
    EEPROMlanguage = EEPROM.readByte(EEPROM_ADDR_LANGUAGE);
    
    return EEPROMlanguage;
}

byte Settings::loadBrightnessFromEEPROM()
{
    byte EEPROMbrightness;
    
    EEPROMbrightness = EEPROM.readByte(EEPROM_ADDR_BRIGHTNESS);
    
    return EEPROMbrightness;
}

pixel_t Settings::loadColorFromEEPROM()
{
    pixel_t EEPROMcolor;

    EEPROMcolor.red = EEPROM.readByte(EEPROM_ADDR_COLORRED);
    EEPROMcolor.green = EEPROM.readByte(EEPROM_ADDR_COLORGREEN);
    EEPROMcolor.blue = EEPROM.readByte(EEPROM_ADDR_COLORBLUE);
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Farbe: ");
        Serial.print("rot: ");
        Serial.print(EEPROMcolor.red);
        Serial.print(" gruen: ");
        Serial.print(EEPROMcolor.green);
        Serial.print(" blau: ");
        Serial.print(EEPROMcolor.blue);
        Serial.print(" wird von den Adressen ");
        Serial.print("rot: ");
        Serial.print(EEPROM_ADDR_COLORRED);
        Serial.print(" gruen: ");
        Serial.print(EEPROM_ADDR_COLORGREEN);
        Serial.print(" blau: ");
        Serial.print(EEPROM_ADDR_COLORBLUE);
        Serial.println(" gelesen");
    }
    return EEPROMcolor;
}

byte Settings::loadFadeModeFromEEPROM()
{
    byte EEPROMfadeMode;

    EEPROMfadeMode = EEPROM.readByte(EEPROM_ADDR_FADEMODE);

    return EEPROMfadeMode;
}

byte Settings::loadCornerStartLedFromEEPROM()
{
    byte EEPROMcornerStartLed;

    EEPROMcornerStartLed = EEPROM.readByte(EEPROM_ADDR_CORNERSTARTLED);

    return EEPROMcornerStartLed;
}

bool Settings::loadCornerClockwiseFromEEPROM()
{
    bool EEPROMcornerClockwise;

    EEPROMcornerClockwise = EEPROM.readBool(EEPROM_ADDR_CORNERCLOCKWISE);

    return EEPROMcornerClockwise;
}

byte Settings::loadStartpatternFromEEPROM()
{
    byte EEPROMstartpattern;

    EEPROMstartpattern = EEPROM.readByte(EEPROM_ADDR_STARTPATTERN);

    return EEPROMstartpattern;
}

uint16_t Settings::loadGmtOffsetFromEEPROM()
{
    uint16_t EEPROMgmtOffset;

    EEPROMgmtOffset = EEPROM.readUShort(EEPROM_ADDR_GMTOFFSET);

    return EEPROMgmtOffset;
}

/***************************************************************************
 * Schreiben der Sprache auf den EEPROM
 * Übergabeparameter: Sprache, die verwendet werden soll
 * Rückgabeparameter: kein
 **************************************************************************/
void Settings::writeLanguageToEEPROM(byte language)
{
    EEPROM.writeByte(EEPROM_ADDR_LANGUAGE, language);
    EEPROM.commit();
}

void Settings::writeBrightnessToEEPROM(byte brightness)
{
    EEPROM.writeByte(EEPROM_ADDR_BRIGHTNESS, brightness);
    EEPROM.commit();
}

void Settings::writeColorToEEPROM(pixel_t color)
{
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Farbe: ");
        Serial.print("rot: ");
        Serial.print(color.red);
        Serial.print(" gruen: ");
        Serial.print(color.green);
        Serial.print(" blau: ");
        Serial.print(color.blue);
        Serial.print(" wird auf die Adressen ");
        Serial.print("rot: ");
        Serial.print(EEPROM_ADDR_COLORRED);
        Serial.print(" gruen: ");
        Serial.print(EEPROM_ADDR_COLORGREEN);
        Serial.print(" blau: ");
        Serial.print(EEPROM_ADDR_COLORBLUE);
        Serial.println(" geschrieben");
    }
    EEPROM.writeByte(EEPROM_ADDR_COLORRED, color.red);
    EEPROM.writeByte(EEPROM_ADDR_COLORGREEN, color.green);
    EEPROM.writeByte(EEPROM_ADDR_COLORBLUE, color.blue);
    EEPROM.commit();
}

void Settings::writeFadeModeToEEPROM(byte fademode)
{
    EEPROM.writeByte(EEPROM_ADDR_FADEMODE, fademode);
    EEPROM.commit();
}

void Settings::writeCornerStartLedToEEPROM(byte cornerstartled)
{
    EEPROM.writeByte(EEPROM_ADDR_CORNERSTARTLED, cornerstartled);
    EEPROM.commit();
}

void Settings::writeCornerClockwiseToEEPROM(bool cornersclockwise)
{
    EEPROM.writeBool(EEPROM_ADDR_CORNERCLOCKWISE, cornersclockwise);
    EEPROM.commit();
}

void Settings::writeStartpatternToEEPROM(byte startpattern)
{
    EEPROM.writeByte(EEPROM_ADDR_STARTPATTERN, startpattern);
    EEPROM.commit();
}

void Settings::writeGmtOffsetToEEPROM(uint16_t gmtoffset)
{
    EEPROM.writeUShort(EEPROM_ADDR_GMTOFFSET, gmtoffset);
    EEPROM.commit();
}
