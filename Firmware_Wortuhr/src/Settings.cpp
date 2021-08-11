/***************************************************************************
 * Funktionen für die Einstellungsmöglichkeiten der Wortuhr
 *
 **************************************************************************/

/* Einbinden von Headerdateien */
#include "Settings.h"

/****************************************
 * Definition Objekte
 ***************************************/
Preferences preferences;

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
String Settings::_SSID;
String Settings::_Password;

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
    _StartPattern = 1;
    _GmtTimeOffsetSec = 3600;
    _SSID = "";
    _Password = "";
}

/****************************************
 * Sprache
 ***************************************/
void Settings::setLanguage(byte Language)
{
    if(_Language != Language)
    {
        writeLanguageToPreferences(Language);
        loadLanguageFromPreferences();
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
        writeBrightnessToPreferences(map(Brightness, 0, 100, 0, 255));
        loadBrightnessFromPreferences();
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
        writeColorToPreferences(color);
        loadColorFromPreferences();
    }
}

void Settings::setColor(byte red, byte green, byte blue)
{
    if((_Color.red != red) || (_Color.green != green) || (_Color.blue != blue))
    {
        writeColorToPreferences(red, green, blue);
        loadColorFromPreferences();
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
        writeFadeModeToPreferences(FadeMode);
        loadFadeModeFromPreferences();
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
        writeCornerStartLedToPreferences(CornerStartLed);
        loadCornerStartLedFromPreferences();
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
        writeCornerClockwiseToPreferences(CornersClockwise);
        loadCornerClockwiseFromPreferences();
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
        writeStartpatternToPreferences(StartPattern);
        loadStartpatternFromPreferences();
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
        writeGmtOffsetToPreferences(GmtTimeOffsetSec);
        loadGmtOffsetFromPreferences();
    }
}

uint16_t Settings::getGmtTimeOffsetSec()
{
    return _GmtTimeOffsetSec;
}

/***************************************************************************
 * Auslesen der Sprache aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _Language gespeichert werden
 **************************************************************************/
void Settings::loadLanguageFromPreferences()
{
    preferences.begin("settings", true);
    _Language = preferences.getUChar("language");
    preferences.end();
}

/***************************************************************************
 * Auslesen der Helligkeit aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _Brightness gespeichert werden
 **************************************************************************/
void Settings::loadBrightnessFromPreferences()
{
    preferences.begin("settings", true);
    _Brightness = preferences.getUChar("brightness");
    preferences.end();
}

/***************************************************************************
 * Auslesen der Farbe aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _Color gespeichert werden
 **************************************************************************/
void Settings::loadColorFromPreferences()
{
    preferences.begin("settings", true);
    _Color.red = preferences.getUChar("colorred");
    _Color.green = preferences.getUChar("colorgreen");
    _Color.blue = preferences.getUChar("colorblue");
    preferences.end();
}

/***************************************************************************
 * Auslesen des FadeMode aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _FadeMode gespeichert werden
 **************************************************************************/
void Settings::loadFadeModeFromPreferences()
{
    preferences.begin("settings", true);
    _FadeMode = preferences.getUChar("fademode");
    preferences.end();
}

/***************************************************************************
 * Auslesen des Start Eck-LED aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _CornerStartLed gespeichert werden
 **************************************************************************/
void Settings::loadCornerStartLedFromPreferences()
{
    preferences.begin("settings", true);
    _CornerStartLed = preferences.getUChar("cornerstartled");
    preferences.end();
}

/***************************************************************************
 * Auslesen der Drehrichtung der Eck-LED aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _CornersClockwise gespeichert werden
 **************************************************************************/
void Settings::loadCornerClockwiseFromPreferences()
{
    preferences.begin("settings", true);
    _CornersClockwise = preferences.getBool("cornerclockwise");
    preferences.end();
}

/***************************************************************************
 * Auslesen des Startmusters aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _StartPattern gespeichert werden
 **************************************************************************/
void Settings::loadStartpatternFromPreferences()
{
    preferences.begin("settings", true);
    _StartPattern = preferences.getUChar("startpattern");
    preferences.end();
}

/***************************************************************************
 * Auslesen des GMT Offsets aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _GmtTimeOffsetSec gespeichert werden
 **************************************************************************/
void Settings::loadGmtOffsetFromPreferences()
{
    preferences.begin("settings", true);
    _GmtTimeOffsetSec = preferences.getUShort("gmtoffset");
    preferences.end();
}

/***************************************************************************
 * Auslesen der WiFi SSID aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _GmtTimeOffsetSec gespeichert werden
 **************************************************************************/
void Settings::loadSsidFromPreferences()
{
    preferences.begin("settings", true);
    _SSID = preferences.getString("wifissid");
    preferences.end();
}

/***************************************************************************
 * Auslesen des WiFi Passworts aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _GmtTimeOffsetSec gespeichert werden
 **************************************************************************/
void Settings::loadPasswordFromPreferences()
{
    preferences.begin("settings", true);
    _Password = preferences.getString("wifipassword");
    preferences.end();
}

/***************************************************************************
 * Schreiben der Sprache in Preferences
 * Übergabeparameter: Sprache
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeLanguageToPreferences(byte language)
{
    preferences.begin("settings", false);
    preferences.putUChar("language", language);
    preferences.end();
}

/***************************************************************************
 * Schreiben der Helligkeit in Preferences
 * Übergabeparameter: Helligkeitswert
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeBrightnessToPreferences(byte brightness)
{
    preferences.begin("settings", false);
    preferences.putUChar("brightness", brightness);
    preferences.end();
}

/***************************************************************************
 * Schreiben der Farbe in Preferences
 * Übergabeparameter: Farbe als pixel_t struct
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeColorToPreferences(pixel_t color)
{
    preferences.begin("settings", false);
    preferences.putUChar("colorred", color.red);
    preferences.putUChar("colorgreen", color.green);
    preferences.putUChar("colorblue", color.blue);
    preferences.end();
}

/***************************************************************************
 * Schreiben der Farbe in Preferences
 * Übergabeparameter: Farbe als RGB
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeColorToPreferences(byte red, byte green, byte blue)
{
    preferences.begin("settings", false);
    preferences.putUChar("colorred", red);
    preferences.putUChar("colorgreen", green);
    preferences.putUChar("colorblue", blue);
    preferences.end();
}

/***************************************************************************
 * Schreiben des FadeMode in Preferences
 * Übergabeparameter: FadeMode
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeFadeModeToPreferences(byte fademode)
{
    preferences.begin("settings", false);
    preferences.putUChar("fademode", fademode);
    preferences.end();
}

/***************************************************************************
 * Schreiben der Start Eck-LED in Preferences
 * Übergabeparameter: Start Eck-LED
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeCornerStartLedToPreferences(byte cornerstartled)
{
    preferences.begin("settings", false);
    preferences.putUChar("cornerstartled", cornerstartled);
    preferences.end();
}

/***************************************************************************
 * Schreiben der Drehrichtung der Eck-LEDs in Preferences
 * Übergabeparameter: Drechrichtung Eck-LED
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeCornerClockwiseToPreferences(bool cornersclockwise)
{
    preferences.begin("settings", false);
    preferences.putBool("cornerclockwise", cornersclockwise);
    preferences.end();
}

/***************************************************************************
 * Schreiben des Startmusters in Preferences
 * Übergabeparameter: Startmuster
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeStartpatternToPreferences(byte startpattern)
{
    preferences.begin("settings", false);
    preferences.putUChar("startpattern", startpattern);
    preferences.end();
}

/***************************************************************************
 * Schreiben des Startmusters in Preferences
 * Übergabeparameter: Startmuster
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeGmtOffsetToPreferences(uint16_t gmtoffset)
{
    preferences.begin("settings", false);
    preferences.putUShort("gmtoffset", gmtoffset);
    preferences.end();
}

/***************************************************************************
 * Schreiben der Wifi SSID in Preferences
 * Übergabeparameter: SSID Name
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeSsidToPreferences(String ssid)
{
    preferences.begin("settings", false);
    preferences.putString("wifissid", ssid);
    preferences.end();
}


/***************************************************************************
 * Schreiben des Wifi PW in Preferences
 * Übergabeparameter: Passwort
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writePasswordToPreferences(String password)
{
    preferences.begin("settings", false);
    preferences.putString("wifipassword", password);
    preferences.end();
}


/***************************************************************************
 * Auslesen aller Einstellungen aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: Farbe als pixel_t struct
 **************************************************************************/
void Settings::loadDataFromPreferences()
{
    loadLanguageFromPreferences();
    loadBrightnessFromPreferences();
    loadColorFromPreferences();
    loadFadeModeFromPreferences();
    loadCornerStartLedFromPreferences();
    loadCornerClockwiseFromPreferences();
    loadStartpatternFromPreferences();
    loadGmtOffsetFromPreferences();
    if(DEBUG_SETTINGS)
    {
        Serial.println("Geladene Einstellungen:");
        Serial.print("Sprache: ");
        Serial.println(_Language);
        Serial.print("Helligkeit: ");
        Serial.println(_Brightness);
        Serial.print("Farbe - rot: ");
        Serial.println(_Color.red);
        Serial.print("Farbe - green: ");
        Serial.println(_Color.green);
        Serial.print("Farbe - blue: ");
        Serial.println(_Color.blue);
        Serial.print("FadeMode: ");
        Serial.println(_FadeMode);
        Serial.print("Start Eck-LED: ");
        Serial.println(_CornerStartLed);
        Serial.print("Drehrichtung Eck-Leds: ");
        Serial.println(_CornersClockwise);
        Serial.print("Startmuster: ");
        Serial.println(_StartPattern);
        Serial.print("GMT Offset: ");
        Serial.println(_GmtTimeOffsetSec);
        Serial.print("WiFi SSID: ");
        if(_SSID = "")
        {
            Serial.println("empty");
        }
        else
        {
            Serial.println(_SSID);
        }
        Serial.print("WiFi Passwort: ");
        if(_Password = "")
        {
            Serial.println("empty");
        }
        else
        {
            Serial.println(_Password);
        }
    }
}

/***************************************************************************
 * Schreiben aller Einstellungen in Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Klassenobjekten gelesen werden
 * Rückgabeparameter: kein, da die Objekte direkt in die Preferences geschrieben werden
 **************************************************************************/
void Settings::writeDataToPreferences()
{
    writeLanguageToPreferences(_Language);
    writeBrightnessToPreferences(_Brightness);
    writeColorToPreferences(_Color);
    writeFadeModeToPreferences(_FadeMode);
    writeCornerStartLedToPreferences(_CornerStartLed);
    writeCornerClockwiseToPreferences(_CornersClockwise);
    writeStartpatternToPreferences(_StartPattern);
    writeGmtOffsetToPreferences(_GmtTimeOffsetSec);
    writeSsidToPreferences(_SSID);
    writePasswordToPreferences(_Password);
    
}