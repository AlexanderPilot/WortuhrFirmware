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
void AppInterpreter::readCommandCharFromApp(char CommandChar)
{
    // static Variablen müssen initialisiert werden
    static char _AppBefehlBuffer[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'}; //9 Elemente
    char _AppBefehl[] = {'0', '0', '0', '0', '0', '0', '0'};                             //6 Elemente
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
        //Erstellung lokale Kopie für Befehle inkl Drehung der Orientierung
        for (i = (LENGTH_COMMAND_BUFFER - 3); i > 1; i--)
        {
            _AppBefehl[7 - i] = _AppBefehlBuffer[i];
            //Serial.print(_AppBefehl[8 - i]);
        }

        switch (_AppBefehlBuffer[POS_COMMAND_SIGN])
        {
        case SIGN_PASSWORD: // Auswerten Password
            if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_CONTINUE)
            {
                if (DEBUG_APPINTERPRETER == 1)
                {
                    Serial.print("PW + ");
                    _justSendTheFoundStringToSerial(_AppBefehlBuffer);
                }
                _CommSetPW(_AppBefehl, true);
            }
            else if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL)
            {
                if (DEBUG_APPINTERPRETER == 1)
                {
                    Serial.print("PW (END)");
                    _justSendTheFoundStringToSerial(_AppBefehlBuffer);
                }
                _CommSetPW(_AppBefehl, false);
            }
            break;
        case SIGN_SSID: // Auswerten SSID
            if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_CONTINUE)
            {
                if (DEBUG_APPINTERPRETER == 1)
                {
                    Serial.print("AppInterpreter.cpp - SSID + ");
                    _justSendTheFoundStringToSerial(_AppBefehlBuffer);
                }
                _CommSetSSID(_AppBefehl, true);
            }
            else if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL)
            {
                if (DEBUG_APPINTERPRETER == 1)
                {
                    Serial.print("AppInterpreter.cpp - SSID (END)");
                    _justSendTheFoundStringToSerial(_AppBefehlBuffer);
                }
                _CommSetSSID(_AppBefehl, false);
            }
            break;
        case SIGN_COLOR: // Auswerten der Farbe
            if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL)
            {
                if (DEBUG_APPINTERPRETER == 1)
                {
                    Serial.print("AppInterpreter.cpp - Farbe erkannt ");
                    _justSendTheFoundStringToSerial(_AppBefehlBuffer);
                }
                _CommSetColor(_AppBefehl);
            }
            break;
        case SIGN_BRIGHTNESS: // Auswerten der Helligkeit
            if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL)
            {
                if (DEBUG_APPINTERPRETER == 1)
                {
                    Serial.print("AppInterpreter.cpp - Helligkeit erkannt ");
                    _justSendTheFoundStringToSerial(_AppBefehlBuffer);
                }
                _CommSetBrightness(_AppBefehl);
            }
            break;
        case SIGN_CLOCK: // Auswerten der Zeit
            if (_AppBefehlBuffer[POS_SIGN_END] == SIGN_END_ALL)
            {
                if (DEBUG_APPINTERPRETER == 1)
                {
                    Serial.print("AppInterpreter.cpp - Zeit erkannt ");
                    _justSendTheFoundStringToSerial(_AppBefehlBuffer);
                }
                _CommSetTime(_AppBefehl);
            }
            break;
        default:
            break;
        }
    }
}

/***************************************************************************
 * Funktion aus dem App-Befehl die Farbe zu setzen
 * Übergabeparameter: Array mit dem entsprechenden Befehl
 * Rückgabe: kein
***************************************************************************/
void AppInterpreter::_CommSetColor(char *Farbe)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Setzen der LED Farben ");
        for (uint8_t i = 0; i < LENGTH_COMMAND_APP; i++)
        {
            Serial.print(*(Farbe + i));
        }
        Serial.println("");
    }

    pixel_t AppColor;

    //Auslesen der Farbe
    AppColor.red = _hexcharToUint8_t(*Farbe) * 16 + _hexcharToUint8_t(*(Farbe + 1));
    AppColor.green = _hexcharToUint8_t(*(Farbe + 2)) * 16 + _hexcharToUint8_t(*(Farbe + 3));
    AppColor.blue = _hexcharToUint8_t(*(Farbe + 4)) * 16 + _hexcharToUint8_t(*(Farbe + 5));

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
void AppInterpreter::_CommSetBrightness(char *Helligkeit)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Setzen der LED Helligkeit");
        for (uint8_t i = 0; i < LENGTH_COMMAND_APP; i++)
        {
            Serial.print(*(Helligkeit + i));
        }
        Serial.println("");
    }

    uint8_t AppBrightness;

    //Auslesen der Helligkeit
    AppBrightness = _hexcharToUint8_t(*Helligkeit) * 100 + _hexcharToUint8_t(*(Helligkeit + 1)) * 10 + _hexcharToUint8_t(*(Helligkeit + 2));
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
void AppInterpreter::_CommSetTime(char *Uhrzeit)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("Setzen der Zeit in RTC: ");
        for (uint8_t i = 0; i < LENGTH_COMMAND_APP; i++)
        {
            Serial.print(*(Uhrzeit + i));
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
    AppHours = _hexcharToUint8_t(*Uhrzeit) * 10 + _hexcharToUint8_t(*(Uhrzeit + 1));

    //Auslesen Minuten
    AppMinutes = _hexcharToUint8_t(*(Uhrzeit + 2)) * 10 + _hexcharToUint8_t(*(Uhrzeit + 3));

    //Auslesen Sekunden
    AppSeconds = _hexcharToUint8_t(*(Uhrzeit + 4)) * 10 + _hexcharToUint8_t(*(Uhrzeit + 5));

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
void AppInterpreter::_CommSetMisc(char *Misc)
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
void AppInterpreter::_CommSetSSID(char *partialSSID, bool continueCommand)
{
    //static char SSID[] = {"OnLine"};
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("WIFI SSID schreiben");
    }
    //_interpretersettings.setWifiSSID(SSID);
    //TODO: programm function
}

/***************************************************************************
 * Funktion aus dem App-Befehl das WiFi Passwort zu setzen
 * Übergabeparameter: Array mit dem entsprechenden Befehl; Information ob weiterer Befehl folgt
 * Rückgabe: kein
***************************************************************************/
void AppInterpreter::_CommSetPW(char *partialPW, bool continueCommand)
{
    static char PWchar;
    static char PW[] = {'0'};
    static uint8_t counter = 0;
    PWchar = this->_getDecryptedChar(partialPW, 5);
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.print("WIFI PW schreiben");
        Serial.print(" ");
        Serial.print(PWchar);
        Serial.print(" Counter: ");
        Serial.print(counter);
        Serial.print(" continue: ");
        Serial.println(continueCommand);
    }

    //Array wird befüllt
    PW[counter] = PWchar;
    counter++;

    //sobald Array befüllt  ist, wird die Einstellung geschrieben
    if (continueCommand == false)
    {
        if (DEBUG_APPINTERPRETER == 1)
        {
            Serial.print("PW vollständig übermittelt: ");
            //Array PW ausgeben
            for (uint8_t i = 0; i < counter; i++)
            {
                Serial.print(PW[i]);
            }
            Serial.println(" ");
        }
        //_interpretersettings.setWifiPW(PW);
        //char PW[] = {'0'};
    }
}

/***************************************************************************
 * Rückgabe des entschlüsselten PW oder SSID characters
 * Übergabeparameter: pointer auf übergebenes Array
 * Rückgabe: entschlüsselter char
***************************************************************************/
char AppInterpreter::_getDecryptedChar(char *c, uint8_t i)
{
    return *(c + i);
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
    //TODO: Funktionsaufruf
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

void AppInterpreter::_setWifiSSID(char *Ssid)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe der WIFI SSID an die Einstellungen-Klasse");
    }

    //_interpretersettings.setWifiSSID(Ssid);
}

void AppInterpreter::_setWifiPW(char *Password)
{
    if (DEBUG_APPINTERPRETER == 1)
    {
        Serial.print("AppInterpreter.cpp - ");
        Serial.println("Uebergabe des WIFI Passwords an die Einstellungen-Klasse");
    }

    //_interpretersettings.setWifiPW(Password);
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
