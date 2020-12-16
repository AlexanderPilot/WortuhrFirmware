

#include "AppInterpreter.h"

Settings _interpretersettings;
Zeitmaster *_interpreterzeitmaster;

AppInterpreter::AppInterpreter()
{
}

/** ===========================================================
 *  | Befehlsstruktur:                                        |
 *  |-----------------+---------------------+-----------------|
 *  | Start/Erkennung | Eigentlicher Befehl | Ende            |
 *  |-----------------+---------------------+-----------------+
 *  | "X"             | # X X X X X X       | '$\n' oder '$\t |
 *  |-----------------+---------------------+-----------------|
 *  
 *  Details in der Befehlsliste
*/

/***************************************************************************
 * Zusammenfügen der einzelnen übertragenen char aus der App in Befehlsbuffer
 * Übergabeparameter: char aus der seriellen BT Übertragung
 * Rückgabe: uint8_t zur Anzeige ob Befehl aktiv war oder nicht
***************************************************************************/
uint8_t AppInterpreter::readCommandCharFromApp(char CommandChar)
{
    // static Variablen müssen initialisiert werden
    static char _AppBefehlBuffer[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'}; //9 Elemente
    char _AppBefehl[] = {'0', '0', '0', '0', '0', '0', '0'};                             //6 Elemente
    uint8_t iRet = 0;
    uint8_t i;

    //Befüllung des Ringbuffers (kopieren von vorne nach hinten, beginnend am Ende)
    for (i = (LENGTH_COMMAND_BUFFER - 1); i > 0; i--)
    {
        _AppBefehlBuffer[i] = _AppBefehlBuffer[i - 1];
    }

    // Neues Zeichen in den Buffer[0] schieben
    _AppBefehlBuffer[0] = CommandChar;

    // Prüfe, ob ein befehl anliegt
    if ((_AppBefehlBuffer[POS_SIGN_STARTCOMMAND] == SIGN_STARTCOMMAND) && (_AppBefehlBuffer[POS_SIGN_ENDCOMMAND] == SIGN_ENDCOMMAND) && ((_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL) || (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_CONTINUE)))
    {
        // Vorläufig wurde was erkannt
        iRet = 1;

        //Erstellung lokale Kopie für Befehle inkl Drehung der Orientierung
        for (i = (LENGTH_COMMAND_BUFFER - 3); i > 1; i--)
        {
            _AppBefehl[7 - i] = _AppBefehlBuffer[i];
            //Serial.print(_AppBefehl[8 - i]);
        }

        switch (_AppBefehlBuffer[POS_COMMAND_SIGN])
        {
        // Auswerten Password
        case SIGN_PASSWORD:
            if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_CONTINUE)
            {
                // Add new character to the string array and wait for more
                Serial.print("PW + ");
                _justSendTheFoundStringToSerial(_AppBefehlBuffer);
            }
            else if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL)
            {
                // Add new character to the string array and use the password
                Serial.print("PW (END)");
                _justSendTheFoundStringToSerial(_AppBefehlBuffer);
            }
            else
            {
                iRet = 0;
            }
            break;
        // Auswerten SSID
        case SIGN_SSID:
            if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_CONTINUE)
            {
                // Add new character to the string array and wait for more
                Serial.print("SSID + ");
                _justSendTheFoundStringToSerial(_AppBefehlBuffer);
            }
            else if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL)
            {
                // Add new character to the string array and use the password
                Serial.print("SSID (END)");
                _justSendTheFoundStringToSerial(_AppBefehlBuffer);
            }
            else
            {
                iRet = 0;
            }
            break;
        // Auswerten der Farbe
        case SIGN_COLOR:
            if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL)
            {
                if (DEBUG_APPINTERPRETER == 1)
                {
                    Serial.print("Farbe erkannt ");
                    _justSendTheFoundStringToSerial(_AppBefehlBuffer);
                }
                _CommSetColor(_AppBefehl);
            }
            else
                iRet = 0;
            break;
        // Auswerten der Helligkeit
        case SIGN_BRIGHTNESS:
            if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL)
            {
                if (DEBUG_APPINTERPRETER == 1)
                {
                    Serial.print("Helligkeit erkannt ");
                    _justSendTheFoundStringToSerial(_AppBefehlBuffer);
                }
                //TODO: Funktion schreiben
            }
            else
                iRet = 0;
            break;
            // Auswerten der Zeit
        case SIGN_CLOCK:
            if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL)
            {
                if (DEBUG_APPINTERPRETER == 1)
                {
                    Serial.print("Zeit erkannt ");
                    _justSendTheFoundStringToSerial(_AppBefehlBuffer);
                }
                _CommSetTime(_AppBefehl);
            }
            else
                iRet = 0;
            break;
        default:
            iRet = 0;
            break;
        }
    }

    // Trigger um ein Ereignis auszulösen
    return iRet;
}

/***************************************************************************
 * Funktion aus dem App-Befehl die Farbe zu setzen
 * Übergabeparameter: Array mit dem entsprechenden Befehl
 * Rückgabe: kein
***************************************************************************/
void AppInterpreter::_CommSetColor(char AppBefehl[6])
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Setzen der LED Farben");
    }

    pixel_t AppColor;

    //Auslesen der Farbe
    AppColor.red = _hexcharToUint8_t(AppBefehl[0]) * 16 + _hexcharToUint8_t(AppBefehl[1]);
    AppColor.green = _hexcharToUint8_t(AppBefehl[2]) * 16 + _hexcharToUint8_t(AppBefehl[3]);
    AppColor.blue = _hexcharToUint8_t(AppBefehl[4]) * 16 + _hexcharToUint8_t(AppBefehl[5]);

    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("Farbe");
        Serial.print(" rot: ");
        Serial.print(AppColor.red);
        Serial.print(" grün: ");
        Serial.print(AppColor.green);
        Serial.print(" blau: ");
        Serial.println(AppColor.blue);
    }
    //Schrieben der Farbe in die Einstellungen
    _interpretersettings.setColor(AppColor);
}

/***************************************************************************
 * Funktion aus dem App-Befehl die Helligkeit zu setzen
 * Übergabeparameter: Array mit dem entsprechenden Befehl
 * Rückgabe: kein
***************************************************************************/
void AppInterpreter::_CommSetBrightness(char AppBefehl[6])
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Setzen der LED Helligkeit");
    }

    uint8_t AppBrightness;

    //Auslesen der Helligkeit
    AppBrightness = _hexcharToUint8_t(AppBefehl[0]) * 100 + _hexcharToUint8_t(AppBefehl[1]) * 10 + _hexcharToUint8_t(AppBefehl[2]);
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("Helligkeit: ");
        Serial.print(AppBrightness);
    }
    //Verwerfen des versendeten Appwerts bei Wert außerhalb des Wertebereichs
    if (AppBrightness > 100)
    {
        AppBrightness = _interpretersettings.getBrightnessPercent();
    }

    //Schreiben der Helligkeit in die Einstellungen
    _interpretersettings.setBrightnessPercent(AppBrightness);
}

/***************************************************************************
 * Funktion aus dem App-Befehl die Uhrzeit zu setzen
 * Übergabeparameter: Array mit dem entsprechenden Befehl
 * Rückgabe: kein
***************************************************************************/
void AppInterpreter::_CommSetTime(char AppBefehl[6])
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Setzen der Zeit in RTC");
        for (uint8_t i = 0; i < 6; i++)
        {
            Serial.print(AppBefehl[i]);
        }
        Serial.println("");
    }

    uint8_t AppHours;
    uint8_t AppMinutes;
    uint8_t AppSeconds;
    uint8_t AppDate;
    uint8_t AppMonth;
    uint8_t AppYear;

    //Auslesen Stunden
    AppHours = _hexcharToUint8_t(AppBefehl[0]) * 10 + _hexcharToUint8_t(AppBefehl[1]);

    //Auslesen Minuten
    AppMinutes = _hexcharToUint8_t(AppBefehl[2]) * 10 + _hexcharToUint8_t(AppBefehl[3]);

    //Auslesen Sekunden
    AppSeconds = _hexcharToUint8_t(AppBefehl[4]) * 10 + _hexcharToUint8_t(AppBefehl[5]);

    //Verwerfen der versendeten Appwerte bei Werten außerhalb des Wertebereichs
    if ((AppHours > 23) || (AppMinutes > 59) || (AppSeconds > 59))
    {
        AppHours = _interpreterzeitmaster->getHours();
        AppMinutes = _interpreterzeitmaster->getMinutes();
        AppSeconds = _interpreterzeitmaster->getSeconds();
    }

    //Auslesen der bereits enthaltenen Datumsinformation
    AppDate = _interpreterzeitmaster->getDate();
    AppMonth = _interpreterzeitmaster->getMonth();
    AppYear = _interpreterzeitmaster->getYear();

    //Schreiben der Uhrzeit auf die Echtzeituhr
    _interpreterzeitmaster->setTimeDate(AppHours, AppMinutes, AppSeconds, AppDate, AppMonth, AppYear);
}

/***************************************************************************
 * Funktion aus dem App-Befehl verschiedene Funktionen zu auszulösen //TODO
 * Übergabeparameter: Array mit dem entsprechenden Befehl
 * Rückgabe: kein
***************************************************************************/
void AppInterpreter::_CommSetMisc(char AppBefehl[6])
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Diverse Befehle");
    }
}

/***************************************************************************
 * Funktion aus dem App-Befehl die WiFi SSID zu setzen
 * Übergabeparameter: Array mit dem entsprechenden Befehl; Information ob weiterer Befehl folgt
 * Rückgabe: kein
***************************************************************************/
void AppInterpreter::_CommSetSSID(char AppBefehl[6], bool continueCommand)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("WIFI SSID schreiben");
    }
    //TODO: programm function
}

/***************************************************************************
 * Funktion aus dem App-Befehl das WiFi Passwort zu setzen
 * Übergabeparameter: Array mit dem entsprechenden Befehl; Information ob weiterer Befehl folgt
 * Rückgabe: kein
***************************************************************************/
void AppInterpreter::_CommSetPW(char AppBefehl[6], bool continueCommand)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("WIFI PW schreiben");
    }
    //TODO: programm function
}

/***************************************************************************
     * Serielle Ausgabe des Befehls
     * Übergabeparameter: pointer auf Beginn des Buffer-Arrays
     * Rückgabe: kein
***************************************************************************/
void AppInterpreter::_justSendTheFoundStringToSerial(char *p)
{
    uint8_t i;
    for (i = (LENGTH_COMMAND_APP + 1); i > 1; i--)
    {
        Serial.print(*(p + i));
    }
    Serial.print('\n');
}

/****************************************
 * Einstellungen vom Mikrocontroller lesen
 ***************************************/
void AppInterpreter::_loadSettingsFromUC()
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Abrufen der Einstellungen vom Mikrocontroller - TBD");
    }
}

byte AppInterpreter::_getLanguage()
{

    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Sprache aus der Einstellungen-Klasse");
    }

    return _interpretersettings.getLanguage();
}

byte AppInterpreter::_getBrightnessPercent()
{

    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Helligkeit aus der Einstellungen-Klasse");
    }

    return _interpretersettings.getBrightnessPercent();
}

pixel_t AppInterpreter::_getColor()
{

    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Farbe aus der Einstellungen-Klasse");
    }

    return _interpretersettings.getColor();
}

byte AppInterpreter::_getFadeMode()
{

    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen des Fademodus aus der Einstellungen-Klasse");
    }

    return _interpretersettings.getFadeMode();
}

byte AppInterpreter::_getCornerStartLed()
{

    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Ecke der Start LED aus der Einstellungen-Klasse");
    }

    return _interpretersettings.getCornerStartLed();
}

boolean AppInterpreter::_getCornersClockwise()
{

    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen der Laufrichtung der Eck-LEDs aus der Einstellungen-Klasse");
    }

    return _interpretersettings.getCornersClockwise();
}

byte AppInterpreter::_getStartPattern()
{

    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Auslesen des Startmusters aus der Einstellungen-Klasse");
    }

    return _interpretersettings.getStartPattern();
}

uint16_t AppInterpreter::_getGmtTimeOffsetSec()
{
    if (DEBUG_APPINTERPRETER == 1)
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
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der Sprache an die Einstellungen-Klasse");
    }

    _interpretersettings.setLanguage(Language);
}

void AppInterpreter::_setBrightnessPercent(byte Brightness)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der Helligkeit an die Einstellungen-Klasse");
    }

    _interpretersettings.setBrightnessPercent(Brightness);
}

void AppInterpreter::_setColor(pixel_t color)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der Farbe an die Einstellungen-Klasse");
    }

    _interpretersettings.setColor(color);
}

void AppInterpreter::_setFadeMode(byte fadeMode)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe des Fademodus an die Einstellungen-Klasse");
    }

    _interpretersettings.setFadeMode(fadeMode);
}

void AppInterpreter::_setCornerStartLed(byte CornerStartLed)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der Ecke der Start LED an die Einstellungen-Klasse");
    }

    _interpretersettings.setCornerStartLed(CornerStartLed);
}

void AppInterpreter::_setCornersClockwise(boolean Clockwise)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der Laufrichtung der Eck LEDs an die Einstellungen-Klasse");
    }

    _interpretersettings.setCornersClockwise(Clockwise);
}

void AppInterpreter::_setWifiSSID(String Ssid)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der WIFI SSID an die Einstellungen-Klasse");
    }

    _interpretersettings.setWifiSSID(Ssid);
}

void AppInterpreter::_setWifiPW(String Password)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe des WIFI Passwords an die Einstellungen-Klasse");
    }

    _interpretersettings.setWifiPW(Password);
}

void AppInterpreter::_setStartPattern(byte StartPattern)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe des Startmusters an die Einstellungen-Klasse");
    }

    _interpretersettings.setStartPattern(StartPattern);
}

void AppInterpreter::_setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec)
{
    if (DEBUG_APPINTERPRETER == 1)
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
    if (CONV_VERSION == 1)
    {
        var = map(ArrayData, 0, 0xFFFFFF, 0, 0xFF);
    }
    else if (CONV_VERSION == 2)
    {
        var = ArrayData & 0xFF;
    }

    return var;
}

bool AppInterpreter::_convertVarToBool(uint32_t ArrayData)
{
    bool var = false;
    if (CONV_VERSION == 1)
    {
        if (ArrayData >= 1)
        {
            var = true;
        }
    }
    else if (CONV_VERSION == 2)
    {
        var = ArrayData & 0x1;
    }

    return var;
}

uint16_t AppInterpreter::_convertVarToUint16(uint32_t ArrayData)
{
    uint16_t var = 0;

    if (CONV_VERSION == 1)
    {
        var = map(ArrayData, 0, 0xFFFFFF, 0, 0xFFFF);
    }
    else if (CONV_VERSION == 2)
    {
        var = ArrayData & 0xFFFF;
    }

    return var;
}

/***************************************************************************
 * Funktion zum Konvertieren von (hex)char in (dec)uint8_t
 * in:  hexadezimaler char Zeichen
 * out: zugehöriger dezimaler int Wert
 **************************************************************************/
uint8_t AppInterpreter::_hexcharToUint8_t(char hexchar)
{
    if (hexchar >= '0' && hexchar <= '9')
        return hexchar - '0';
    if (hexchar >= 'A' && hexchar <= 'F')
        return hexchar - 'A' + 10;
    if (hexchar >= 'a' && hexchar <= 'f')
        return hexchar - 'a' + 10;
    return -1;
}
