

#include "AppInterpreter.h"

Settings _interpretersettings;

AppInterpreter::AppInterpreter()
{
    
}




/***************************************************************************
 * Funktion zum Einlesen der Befehle
 * in:  je ein Zeichen aus der Bluetooth-Kommunikation
 * out: boolean zur Anzeige wenn ein neues Kommando angekommen ist
 **************************************************************************/
/*bool AppInterpreter::comevatiation(char getChar)
{
  
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
    
}*/

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

/****************************************
 * App Befehle einlesen
 ***************************************/
void AppInterpreter::readCommandCharFromApp(char CommandChar)
{
    bool newCommand = false;
    bool newPWSSIDCommand = false;
    char _AppBefehl[11];
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Zusammenfassen und Plausibilisieren der einzeln übertragenen Char zu einem Array");
    }
    
    //Abfrage ob gültiger Befehl per APP versendet wurde
    if ((_AppBefehl[0] == START_SIGN) && (_AppBefehl[1] == START_SIGN) && (_AppBefehl[2] == START_SIGN) && (_AppBefehl[NUM_COMMAND_COUNT] == END_SIGN))
    {
        newCommand = true; //vollständiger Befehl wurde erkannt
    }
    
    //Aufruf der Funktion zur auswertung des gesamten App Befehls
    if(newCommand == true)
    {
        this->_getCommandFromApp(_AppBefehl);
    }
}


/** =======================================================
 *  | Befehlsstruktur:                                    |
 *  |-----------------+----------------------------+------|
 *  | Start/Erkennung | Eigentlicher Befehl        | Ende |
 *  |-----------------+----------------------------+------+
 *  | "+++"           | Z X X X X X X              | '$\n' |
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

//Funktion für Einstellungen (ohne WIFI_SSID und WIFI_PW)
void AppInterpreter::_getCommandFromApp(char AppBefehl[11])
{
    byte counter;
    pixel_t AppColor;
        
    //Auswertung der Befehle
    switch(AppBefehl[NUM_SIGN_CATEGORY]) //Zeichen der Kategorie
    {
        //Spracheinstellung
        case SIGN_LANGUAGE:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Spracheinstellung: ");
            }
            
            //this->_setLanguage(convertArrayDataToByte(AppBefehl));
            break;
        
        //Helligkeitseinstellung
        case SIGN_BRIGHTNESS:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Helligkeitswert: ");
            }
            
            //this->_setBrightness(convertArrayDataToByte(AppBefehl));
            break;
            
        //Farbeinstellung
        case SIGN_COLOR:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Farbeinstellung");
            }
            
            //Zuordnung der Zeichenkette zur Farbe
            AppColor.red   = ((AppBefehl[NUM_SIGN_CATEGORY+1] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+1] - 'A' + 10)*16) : ((AppBefehl[NUM_SIGN_CATEGORY+1] - '0')*16)) +
                             ((AppBefehl[NUM_SIGN_CATEGORY+2] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+2] - 'A' + 10))    : ((AppBefehl[NUM_SIGN_CATEGORY+2] - '0')));
            AppColor.green = ((AppBefehl[NUM_SIGN_CATEGORY+3] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+3] - 'A' + 10)*16) : ((AppBefehl[NUM_SIGN_CATEGORY+3] - '0')*16)) +
                             ((AppBefehl[NUM_SIGN_CATEGORY+4] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+4] - 'A' + 10))    : ((AppBefehl[NUM_SIGN_CATEGORY+4] - '0')));
            AppColor.blue  = ((AppBefehl[NUM_SIGN_CATEGORY+5] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+5] - 'A' + 10)*16) : ((AppBefehl[NUM_SIGN_CATEGORY+5] - '0')*16)) +
                             ((AppBefehl[NUM_SIGN_CATEGORY+6] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+6] - 'A' + 10))    : ((AppBefehl[NUM_SIGN_CATEGORY+6] - '0')));
            
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print(" rot: ");
                Serial.print(AppColor.red);
                Serial.print(" gruen: ");
                Serial.print(AppColor.green);
                Serial.print(" blau: ");
                Serial.println(AppColor.blue);
            }
            
            this->_setColor(AppColor);
            break;
            
        //Fade Mode Einstellung
        case SIGN_FADEMODE:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("FadeMode");
            }
            
            //this->_setFadeMode(convertArrayDataToByte(AppBefehl));
            break;
            
        //Start Ecke der Ecke-LED Einstellung
        case SIGN_CORNERSTARTLED:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Ecke der Start LED");
            }
            
            //this->_setCornerStartLed(convertArrayDataToByte(AppBefehl));
            break;
            
        case SIGN_CORNERSCLOCKWISE:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Laufrichtung der Eck-LEDs im Uhrzeigersinn");
            }

            //this->_setCornerStartLed(convertArrayDataToBool(AppBefehl));
            break;

        case SIGN_STARTPATTERN:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Startmuster");
            }

            //this->_setStartPattern(convertArrayDataToByte(AppBefehl));
            break;
            
        case SIGN_GMTOFFSET:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("GMT Offset in Sekunden");
            }

            //this->_setGmtTimeOffsetSec(convertArrayDataToUint16(AppBefehl));
            break;
            
        default:
            break;
    }
}


/****************************************
 * Einstellungen vom Mikrocontroller lesen
 ***************************************/
void AppInterpreter::_loadSettingsFromUC()
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Abrufen der Einstellungen vom Mikrocontroller - TBD");
    }
    
    
}

byte AppInterpreter::_getLanguage()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Sprache");
    }
    
    return _interpretersettings.getLanguage();
}

byte AppInterpreter::_getBrightnessPercent()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Helligkeit");
    }
    
    return _interpretersettings.getBrightnessPercent();
}

pixel_t AppInterpreter::_getColor()    
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Farbe");
    }
    
    return _interpretersettings.getColor();
}

byte AppInterpreter::_getFadeMode()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen des Fademodus");
    }
    
    return _interpretersettings.getFadeMode();
}

byte AppInterpreter::_getCornerStartLed()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Ecke der Start LED");
    }
    
    return _interpretersettings.getCornerStartLed();
}

boolean AppInterpreter::_getCornersClockwise()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Laufrichtugn der Eck-LEDs");
    }
    
    return _interpretersettings.getCornersClockwise();
}

byte AppInterpreter::_getStartPattern()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen des Startmusters");
    }
    
    return _interpretersettings.getStartPattern();
}

uint16_t AppInterpreter::_getGmtTimeOffsetSec()
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
void AppInterpreter::_setLanguage(byte Language)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der Sprache");
    }
    
    _interpretersettings.setLanguage(Language);
}

void AppInterpreter::_setBrightnessPercent(byte Brightness)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der Helligkeit");
    }
    
    _interpretersettings.setBrightnessPercent(Brightness);
}

void AppInterpreter::_setColor(pixel_t color)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Uebergabe der Farbe");
    }
    
    _interpretersettings.setColor(color);
}

void AppInterpreter::_setFadeMode(byte fadeMode)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe des Fademodus");
    }
    
    _interpretersettings.setFadeMode(fadeMode);
}

void AppInterpreter::_setCornerStartLed(byte CornerStartLed)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der Ecke der Start LED");
    }
    
    _interpretersettings.setCornerStartLed(CornerStartLed);
}

void AppInterpreter::_setCornersClockwise(boolean Clockwise)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der Laufrichtung der Eck LEDs");
    }
    
    _interpretersettings.setCornersClockwise(Clockwise);
}

void AppInterpreter::_setWifiSSID(String Ssid)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe der WIFI SSID");
    }
    
    _interpretersettings.setWifiSSID(Ssid);
}

void AppInterpreter::_setWifiPW(String Password)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe des WIFI Passwords");
    }
    
    _interpretersettings.setWifiPW(Password);
}

void AppInterpreter::_setStartPattern(byte StartPattern)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe des Startmusters");
    }
    
    _interpretersettings.setStartPattern(StartPattern);
}

void AppInterpreter::_setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Übergabe des GMT Offsets");
    }
    
    _interpretersettings.setGmtTimeOffsetSec(GmtTimeOffsetSec);
}


/****************************************
 * Hilfsfunktionen für Datenkonvertierung
 ***************************************/
byte AppInterpreter::_convertArrayDataToByte(char ArrayData[11])
{
    byte var;
    Serial.println("Funktion _convertArrayDataToByte muss noch implementiert werden");
    return 0;
}

bool AppInterpreter::_convertArrayDataToBool(char ArrayData[11])
{
    bool var = false;
    
    if(ArrayData[NUM_COMMAND_COUNT] >= 1)
    {
        var= true;
    }
    
    return var;
}

uint16_t AppInterpreter::_convertArrayDataToUint16(char ArrayData[11])
{
    uint16_t var = 0;
    Serial.println("Funktion _convertArrayDataToUint16 muss noch implementiert werden");
    return 0;
}
