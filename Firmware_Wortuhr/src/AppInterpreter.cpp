

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

/****************************************
 * App Befehle einlesen
 ***************************************/
void AppInterpreter::readCommandCharFromApp(char CommandChar)
{
    static bool newCommand = false;
    // !!! static bool newPWSSIDCommand = false;
    static char _AppBefehlBuffer[11];
    // !!! static byte counter;
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Zusammenfassen und Plausibilisieren der einzeln übertragenen Char zu einem Array");
    }
    
    
    
    //Abfrage ob gültiger Befehl per APP versendet wurde
    if ((_AppBefehlBuffer[0] == START_SIGN) && (_AppBefehlBuffer[1] == START_SIGN) && (_AppBefehlBuffer[2] == START_SIGN) && (_AppBefehlBuffer[NUM_COMMAND_COUNT-1] == END_SIGN))
    {
        newCommand = true; //vollständiger Befehl wurde erkannt
    }
    
    //Aufruf der Funktion zur auswertung des gesamten App Befehls
    if(newCommand == true)
    {
        this->_getCommandFromApp(_AppBefehlBuffer);
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
    // !!! byte counter;
    pixel_t AppColor;
    uint32_t var;
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Auswertung des App Befehls");
    }
    
    var   = ((AppBefehl[NUM_SIGN_CATEGORY+1] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+1] - 'A' + 10)<<20) : ((AppBefehl[NUM_SIGN_CATEGORY+1] - '0')<<20)) +
            ((AppBefehl[NUM_SIGN_CATEGORY+2] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+2] - 'A' + 10)<<16) : ((AppBefehl[NUM_SIGN_CATEGORY+2] - '0')<<16)) +
            ((AppBefehl[NUM_SIGN_CATEGORY+3] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+3] - 'A' + 10)<<12) : ((AppBefehl[NUM_SIGN_CATEGORY+3] - '0')<<12)) +
            ((AppBefehl[NUM_SIGN_CATEGORY+4] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+4] - 'A' + 10)<<8)  : ((AppBefehl[NUM_SIGN_CATEGORY+4] - '0')<<8)) +
            ((AppBefehl[NUM_SIGN_CATEGORY+5] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+5] - 'A' + 10)<<4)  : ((AppBefehl[NUM_SIGN_CATEGORY+5] - '0')<<4)) +
            ((AppBefehl[NUM_SIGN_CATEGORY+6] >= 'A') ? ((AppBefehl[NUM_SIGN_CATEGORY+6] - 'A' + 10))     : ((AppBefehl[NUM_SIGN_CATEGORY+6] - '0')));
    
    //Auswertung der Befehle
    switch(AppBefehl[NUM_SIGN_CATEGORY]) //Zeichen der Kategorie
    {
        //Spracheinstellung
        case SIGN_LANGUAGE:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Spracheinstellung: ");
                Serial.print(var);
                Serial.print(" Konvertierter Wert: ");
                Serial.println(_convertVarToByte(var));
            }
            
            this->_setLanguage(_convertVarToByte(var));
            break;
        
        //Helligkeitseinstellung
        case SIGN_BRIGHTNESS:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Helligkeitswert: ");
                Serial.print(var);
                Serial.print(" Konvertierter Wert: ");
                Serial.println(_convertVarToByte(var));
            }
            
            this->_setBrightnessPercent(map(_convertVarToByte(var), 0, 255, 0, 100));
            break;
            
        //Farbeinstellung
        case SIGN_COLOR:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Farbeinstellung: ");
            }
            
            //Zuordnung der Zeichenkette zur Farbe
            AppColor.red   = (var & 0xFF0000) >> 16;
            AppColor.green = (var & 0x00FF00) >> 8;
            AppColor.blue   = (var & 0x0000FF);
            
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("rot: ");
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
                Serial.print("FadeMode: ");
                Serial.print(var);
                Serial.print(" Konvertierter Wert: ");
                Serial.println(_convertVarToByte(var));
            }
            
            this->_setFadeMode(_convertVarToByte(var));
            break;
            
        //Start Ecke der Ecke-LED Einstellung
        case SIGN_CORNERSTARTLED:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Ecke der Start LED: ");
                Serial.print(var);
                Serial.print(" Konvertierter Wert: ");
                Serial.println(_convertVarToByte(var));
            }
            
            this->_setCornerStartLed(_convertVarToByte(var));
            break;
            
        case SIGN_CORNERSCLOCKWISE:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Laufrichtung der Eck-LEDs im Uhrzeigersinn: ");
                Serial.print(var);
                Serial.print(" Konvertierter Wert: ");
                Serial.println(_convertVarToBool(var));
            }

            this->_setCornerStartLed(_convertVarToBool(var));
            break;

        case SIGN_STARTPATTERN:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("Startmuster: ");
                Serial.print(var);
                Serial.print(" Konvertierter Wert: ");
                Serial.println(_convertVarToByte(var));
            }

            this->_setStartPattern(_convertVarToByte(var));
            break;
            
        case SIGN_GMTOFFSET:
            if(DEBUG_APPINTERPRETER == 1)
            {
                Serial.print("AppInterpreter.cpp - ");
                Serial.print("GMT Offset in Sekunden: ");
                Serial.print(var);
                Serial.print(" Konvertierter Wert: ");
                Serial.println(_convertVarToUint16(var));
            }

            this->_setGmtTimeOffsetSec(_convertVarToUint16(var));
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
        Serial.print("Auslesen der Sprache aus der Einstellungen-Klasse");
    }
    
    return _interpretersettings.getLanguage();
}

byte AppInterpreter::_getBrightnessPercent()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Helligkeit aus der Einstellungen-Klasse");
    }
    
    return _interpretersettings.getBrightnessPercent();
}

pixel_t AppInterpreter::_getColor()    
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Farbe aus der Einstellungen-Klasse");
    }
    
    return _interpretersettings.getColor();
}

byte AppInterpreter::_getFadeMode()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen des Fademodus aus der Einstellungen-Klasse");
    }
    
    return _interpretersettings.getFadeMode();
}

byte AppInterpreter::_getCornerStartLed()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Ecke der Start LED aus der Einstellungen-Klasse");
    }
    
    return _interpretersettings.getCornerStartLed();
}

boolean AppInterpreter::_getCornersClockwise()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Laufrichtung der Eck-LEDs aus der Einstellungen-Klasse");
    }
    
    return _interpretersettings.getCornersClockwise();
}

byte AppInterpreter::_getStartPattern()
{
    
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen des Startmusters aus der Einstellungen-Klasse");
    }
    
    return _interpretersettings.getStartPattern();
}

uint16_t AppInterpreter::_getGmtTimeOffsetSec()
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen des GMT Offsets aus der Einstellungen-Klasse");
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
        Serial.println("Uebergabe der Sprache an die Einstellungen-Klasse");
    }
    
    _interpretersettings.setLanguage(Language);
}

void AppInterpreter::_setBrightnessPercent(byte Brightness)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der Helligkeit an die Einstellungen-Klasse");
    }
    
    _interpretersettings.setBrightnessPercent(Brightness);
}

void AppInterpreter::_setColor(pixel_t color)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der Farbe an die Einstellungen-Klasse");
    }
    
    _interpretersettings.setColor(color);
}

void AppInterpreter::_setFadeMode(byte fadeMode)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe des Fademodus an die Einstellungen-Klasse");
    }
    
    _interpretersettings.setFadeMode(fadeMode);
}

void AppInterpreter::_setCornerStartLed(byte CornerStartLed)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der Ecke der Start LED an die Einstellungen-Klasse");
    }
    
    _interpretersettings.setCornerStartLed(CornerStartLed);
}

void AppInterpreter::_setCornersClockwise(boolean Clockwise)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der Laufrichtung der Eck LEDs an die Einstellungen-Klasse");
    }
    
    _interpretersettings.setCornersClockwise(Clockwise);
}

void AppInterpreter::_setWifiSSID(String Ssid)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der WIFI SSID an die Einstellungen-Klasse");
    }
    
    _interpretersettings.setWifiSSID(Ssid);
}

void AppInterpreter::_setWifiPW(String Password)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe des WIFI Passwords an die Einstellungen-Klasse");
    }
    
    _interpretersettings.setWifiPW(Password);
}

void AppInterpreter::_setStartPattern(byte StartPattern)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe des Startmusters an die Einstellungen-Klasse");
    }
    
    _interpretersettings.setStartPattern(StartPattern);
}

void AppInterpreter::_setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec)
{
    if(DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe des GMT Offsets an die Einstellungen-Klasse");
    }
    
    _interpretersettings.setGmtTimeOffsetSec(GmtTimeOffsetSec);
}


/****************************************
 * Hilfsfunktionen für Datenkonvertierung
 ***************************************/
byte AppInterpreter::_convertVarToByte(uint32_t ArrayData)
{
    byte var;
    if(CONV_VERSION == 1)
    {
        var = map(ArrayData, 0, 0xFFFFFF, 0, 0xFF);
    }
    else if(CONV_VERSION == 2)
    {
        var = ArrayData & 0xFF;
    }
    
    return var;
}

bool AppInterpreter::_convertVarToBool(uint32_t ArrayData)
{
    bool var = false;
    if(CONV_VERSION == 1)
    {
        if(ArrayData >= 1)
        {
            var = true;
        }
    }
    else if(CONV_VERSION == 2)
    {
        var = ArrayData & 0x1;
    }
    
    return var;
}

uint16_t AppInterpreter::_convertVarToUint16(uint32_t ArrayData)
{
    uint16_t var = 0;
    
    if(CONV_VERSION == 1)
    {
        var = map(ArrayData, 0, 0xFFFFFF, 0, 0xFFFF);
    }
    else if(CONV_VERSION == 2)
    {
        var = ArrayData & 0xFFFF;
    }
    
    return var;
}

void deleteSerialIn()
{
    while( Serial.available() )
        Serial.read();
}

void AppInterpreter::serialTestRead( pixel_t *param )
{
    int8_t i = 0;
    char val;
    uint8_t myColor[3]; 
    while( Serial.available() && ( i < 3 ))
    {
        val = Serial.read();
        if( val != ' ' )
        {
            myColor[i] = (uint8_t)val;
            i ++;
        }
        
        if( i == 2)
        {
            param->red = myColor[0];
            param->green = myColor[1];
            param->blue = myColor[2];
        }
    } 
    deleteSerialIn();
}

u