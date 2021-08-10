/***************************************************************************
 * Funktionen für die Einstellungsmöglichkeiten der Wortuhr
 *
 **************************************************************************/

/* Einbinden von Headerdateien */
#include "WiFiControl.h"

/****************************************
 * Definition der static Variablen
 ***************************************/
char WiFiControl::_SSID[] = "Hallo_SSID";
char WiFiControl::_PW[] = "Hallo_PW";

/****************************************
 * Konstruktor mit Standardeinstellungen
 ***************************************/
WiFiControl::WiFiControl()
{
    //TODO: muss noch erstellt werden
}

char* WiFiControl::getSSID()
{
    return _SSID;
}

void WiFiControl::setSSID(char* ssid)
{
    //TODO: muss noch erstellt werden
}

char* WiFiControl::getPW()
{
    return _PW;
}

void WiFiControl::setPW(char* password)
{
    //TODO: muss noch erstellt werden
}

void WiFiControl::autoReconnectWifi()
{
    //TODO: muss noch erledigt werden
}