/***************************************************************************
 * Header für die Einstellungsmöglichkeiten der Wortuhr
 *
 **************************************************************************/

/** Vermeidung Doppeldefinitionen **/
#pragma once

/** Einbinden von relevanten Bibliotheken **/
#include "Arduino.h"
#include "Configurations.h"

#define DEBUG_WIFICONTROL 0

class WiFiControl
{
public:
    WiFiControl();
    
    char* getSSID();
    void setSSID(char* ssid);
    
    char* getPW();
    void setPW(char* password);
    
    void autoReconnectWifi();
private:
    static char _SSID[20];
    static char _PW[30];
};