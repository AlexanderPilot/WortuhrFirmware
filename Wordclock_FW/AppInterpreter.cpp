

#include "AppInterpreter.h"

Settings _interpretersettings;

/** =======================================================
 *  | Befehlsstruktur:                                    |
 *  |-----------------+----------------------------+------|
 *  | Start/Erkennung | Eigentlicher Befehl        | Ende |
 *  |-----------------+----------------------------+------+
 *  | "+++"           | Z X X X X X X              | '\n' |
 *  |-----------------+----------------------------+------|
 *  
 *  Z entspricht:
 *  '#' Einstellen der Farbe --> char farbe[6] --> RGB-Wert
 *      In Hexadezimalzahlen:
 *      +-------+-------+-------+
 *      | Rot   | Gruen | Blau  |
 *      +-------+-------+-------+
 *      | msb   |       | lsb   |
 *      +-------+-------+-------+
 *      | X X   | X X   | X X   |
 *      |[0][1] |[2][3] |[4][5] |
 *      +-------+-------+-------+
 *      Nur Grossbuchstaben und Zahlen benutzen!
 *  '!' Einstellen der Helligkeit  --> char helligkeit[6] --> In %
 *  	Eingang: Zahl [0 - 999999] --> nachfolgendes Convert in 0 - 100%
 *  '%' Weitere Befehle --> char befehl[6] --> tbd.
 *
 *  X steht fuer beliebige Zeichen, ausser Sonderzeichen
 *  
*/

AppInterpreter::AppInterpreter()
{
    
}




/***************************************************************************
 * Funktion zum Einlesen der Befehle
 * in:  je ein Zeichen aus der Bluetooth-Kommunikation
 * out: boolean zur Anzeige wenn ein neues Kommando angekommen ist
 **************************************************************************/
bool AppInterpreter::comevatiation(char getChar)
{
    /*
    // Indikator ob ein Kommando vollstaendig angekommen ist
    static bool newComAv = false;
    static int zaehler = 0;
    static bool comStart = false;

    if( newComAv )
    {
        zaehler  = 0;
        comStart = false;
        newComAv = false;
    }

    // Neues Kommando einlesen
    if(comStart)
    {
        zaehler++;
        switch(zaehler)
        {
            case 4: 
                if( (getChar == '#') || (getChar == '%') || (getChar == '!') )
                {
                    bufferReader[0] = getChar;
                }
                else
                {
                    comStart = false;
                    zaehler=0;
                }
                break;
            case 5:
                bufferReader[1] = getChar;
                break;
            case 6:
                bufferReader[2] = getChar;
                break;
            case 7:
                bufferReader[3] = getChar;
                break;
            case 8:   
                bufferReader[4] = getChar;
                break;
            case 9:  
                bufferReader[5] = getChar;
                break;
            case 10:  
                bufferReader[6] = getChar;
                break;
            case 11:
                if( getChar == END_SIGN )
                {
                    bufferReader[7] = getChar;
                    newComAv = true;
                    zaehler=0;
                }
                else
                {
                    comStart = false;
                    zaehler=0; 
                }
                break;
            default:
                comStart=false;
                break;
        }
    }

    // Start eines Kommandos feststellen
    if( !(comStart) )
    {
        if( getChar == START_SIGN )
        {
            zaehler++;
            if( zaehler == 3 )
            {
                comStart = true;
            }
        }
        else
        {
            zaehler = 0;
        }
    }
    // ist "true" wenn neues Kommando erkannt wurde
    if(newComAv)
    {
        setUpCommand( );
    }
    return newComAv;
    */
}

/***************************************************************************
 * Funktion zum konvertieren von char zu int --> zum Feststellen
 * der Helligkeit.
 * in:  globale Variable: bufferReader[...]
 * out: unsigned integer zwischen 0 und 100 [%]
 **************************************************************************/
unsigned int charToInt( void )
{
    /*
    unsigned int convert[] = {0,100000,10000,1000,100,10,1};
    unsigned int iRet = 0;
    unsigned int iVal = 1;
    while( iVal < sizeof(bufferReader) )
    {
        iRet = iRet + (bufferReader[iVal] - '0') * convert[iVal];
        iVal ++;
    }
    return ( (iRet+1) * 100 / 999999 );
    */
}

/***************************************************************************
 * Funktion zum Einlesen der Farben
 * in:  globale Variable: bufferReader[...]
 * out: globale Variable: farbe (in HTML Farbcodes-Werten)
 *      globale Variable: farbanteile (Struktur aufgeteil in Rot, Gruen
 *                        und Blau)
 **************************************************************************/
void setUpColor( void )
{
    /*
    // Keinschreibung in Grossschreibung umwandeln
    unsigned int iVal = 1;
    while( iVal < sizeof(bufferReader) )
    {
        if( (bufferReader[iVal] >= 'a') && (bufferReader[iVal] <= 'z') )
            bufferReader[iVal] = bufferReader[iVal] - 32;
        iVal ++;
    }
    iVal = 0;
    while( iVal < sizeof(farbe) )
    {
        farbe[iVal] = bufferReader[iVal+1];
        iVal++;
    }

    farbanteile.red     = 0;
    farbanteile.green   = 0;
    farbanteile.blue    = 0;
    farbanteile.red     = (farbe[0] >= 'A') ? ((farbe[0] - 'A' + 10)*16) : ((farbe[0] - '0')*16);
    farbanteile.red     = (farbe[1] >= 'A') ? (farbanteile.red +(farbe[1] - 'A' + 10)) : (farbanteile.red +(farbe[1] - '0'));
    farbanteile.green   = (farbe[2] >= 'A') ? ((farbe[2] - 'A' + 10)*16) : ((farbe[2] - '0')*16);
    farbanteile.green   = (farbe[3] >= 'A') ? (farbanteile.green+(farbe[3] - 'A' + 10)) : (farbanteile.green+(farbe[3] - '0'));
    farbanteile.blue    = (farbe[4] >= 'A') ? ((farbe[4] - 'A' + 10)*16) : ((farbe[4] - '0')*16);
    farbanteile.blue    = (farbe[5] >= 'A') ? (farbanteile.blue+(farbe[5] - 'A' + 10)) : (farbanteile.blue+(farbe[5] - '0'));
    */
}

/********************************************************************
 * In dieser Funktion werden die Befehle ausgewertet und in die
 * globalen Variablen geschrieben.
 *
 * Input:
 * Globale Variable: bufferReader[6];
 * Output:
 * Globale Variable: helligkeit; (in %)             -> '!'
 * Globale Variable: farbe[6]; (in HTML Farbcodes)  -> '#'
 * Globale Variable: befehl[6]; (tbd.)              -> '%'
 ********************************************************************/
void AppInterpreter::setUpCommand( void )
{
    /*
    switch ( bufferReader[0] )
    {
        case '!':
            helligkeit = charToInt();
            break;
        case '#':
            setUpColor(); break;
        case '%':
            break;
        default:
            break;
    }
    */
}

// Loeschen eines Strings
void AppInterpreter::deleteString(char a[])
{
    uint8_t i = 0;
    for(i=0;i<=sizeof(*a);i++)
    {
    a[i] = '\0';
    }
    //while( sizeof(*a) < i )
    //{
    //  a = '\0';
    //}
}


/****************************************
 * App Befehle einlesen
 ***************************************/
char* AppInterpreter::readCommandCharFromApp(char CommandChar)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("read Command from App");
    }
    
    
}

void AppInterpreter::getCommandFromApp(char AppBefehl[11])
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("get Command from App");
    }
    
    
}


/****************************************
 * Einstellungen vom Mikrocontroller lesen
 ***************************************/
void AppInterpreter::loadSettingsFromUC()
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Abrufen der Einstellungen vom Mikrocontroller - TBD");
    }
    
    
}

byte AppInterpreter::getLanguage()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Sprache");
    }
    
    return _interpretersettings.getLanguage();
}

byte AppInterpreter::getBrightnessPercent()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Helligkeit");
    }
    
    return _interpretersettings.getBrightnessPercent();
}

pixel_t AppInterpreter::getColor()    
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Farbe");
    }
    
    return _interpretersettings.getColor();
}

byte AppInterpreter::getFadeMode()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen des Fademodus");
    }
    
    return _interpretersettings.getFadeMode();
}

byte AppInterpreter::getCornerStartLed()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Ecke der Start LED");
    }
    
    return _interpretersettings.getCornerStartLed();
}

bool AppInterpreter::getCornersClockwise()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Laufrichtugn der Eck-LEDs");
    }
    
    return _interpretersettings.getCornersClockwise();
}

byte AppInterpreter::getStartPattern()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen des Startmusters");
    }
    
    return _interpretersettings.getStartPattern();
}

uint16_t AppInterpreter::getGmtTimeOffsetSec()
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen des GMT Offsets");
    }
    
    return _interpretersettings.getGmtTimeOffsetSec();
}

/****************************************
 * Einstellungen auf Mikrocontroller setzen
 ***************************************/
void AppInterpreter::setLanguage(byte Language)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der Sprache");
    }
    
    _interpretersettings.setLanguage(Language);
}

void AppInterpreter::setBrightnessPercent(byte Brightness)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der Helligkeit");
    }
    
    _interpretersettings.setBrightnessPercent(Brightness);
}

void AppInterpreter::setColor(pixel_t color)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der Farbe");
    }
    
    _interpretersettings.setColor(color);
}

void AppInterpreter::setFadeMode(byte fadeMode)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe des Fademodus");
    }
    
    _interpretersettings.setFadeMode(fadeMode);
}

void AppInterpreter::setCornerStartLed(byte CornerStartLed)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der Ecke der Start LED");
    }
    
    _interpretersettings.setCornerStartLed(CornerStartLed);
}

void AppInterpreter::setCornersClockwise(boolean Clockwise)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der Laufrichtung der Eck LEDs");
    }
    
    _interpretersettings.setCornersClockwise(Clockwise);
}

void AppInterpreter::setWifiSSID(String Ssid)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der WIFI SSID");
    }
    
    _interpretersettings.setWifiSSID(Ssid);
}

void AppInterpreter::setWifiPW(String Password)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe des WIFI Passwords");
    }
    
    _interpretersettings.setWifiPW(Password);
}

void AppInterpreter::setStartPattern(byte StartPattern)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe des Startmusters");
    }
    
    _interpretersettings.setStartPattern(StartPattern);
}

void AppInterpreter::setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe des GMT Offsets");
    }
    
    _interpretersettings.setGmtTimeOffsetSec(GmtTimeOffsetSec);
}

