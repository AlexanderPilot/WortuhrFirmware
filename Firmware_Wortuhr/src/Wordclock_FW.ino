/*****************************************************************************************************************************************************************************************
 * Wordclock Software
 * created: 01.02.2019
 * by Alex P. and Vitali H.
 * 
 * 
 * //TODO:
 * Deaktivierung WLAN nach NTP Sync und Starten vor dem nächsten Sync
 * 
 *****************************************************************************************************************************************************************************************/

/***************************************************************************
 * Einbinden von benötigten Bibliotheken
 **************************************************************************/
#include "Configurations.h"
#include "LED_Ausgabe.h"
#include "Muster.h"

#define STARTINTERRUPT1 (timerAlarmEnable(timer1))
#define STOPINTERRUPT1 (timerAlarmDisable(timer1))

#define STARTINTERRUPT2 (timerAlarmEnable(timer2))
#define STOPINTERRUPT2 (timerAlarmDisable(timer2))

/***************************************************************************
 * Anlegen der Peripherie Instanzen
 **************************************************************************/
BluetoothSerial SerialBT;
Settings settings;
AppInterpreter appinterpreter;
LED_Ausgabe *pLedausgabe;
Zeitmaster *pZeit;
Muster *pMuster;

/***************************************************************************
 * Timer; Eventrtigger
 * ************************************************************************/
hw_timer_t *timer1 = NULL;
hw_timer_t *timer2 = NULL;
bool eventtrigger;
bool ntpSync;
bool wifi_connection_possible = false;
// ISR to set trigger
void IRAM_ATTR onTimer1()
{
    eventtrigger = true;
}

void IRAM_ATTR onTimer2()
{
    ntpSync = true;
}

/* START SETUP ************************************************************/
void setup()
{
    /***************************************************************************
     * Aufbauen der seriellen Kommunikation
     **************************************************************************/
    Serial.begin(SERIAL_SPEED);
    delay(100);
    Serial.println();
    Serial.println("--- Setup gestartet ---");
    
    /***************************************************************************
     * Initialisierung Preferences und Einstellungen
     **************************************************************************/
    if(settings.allDataAvailable() == false)
    {
        Serial.println("Werkseinstellungen werden geschrieben");
        settings.writeDataToPreferences();
        ESP.restart();
    }
    else
    {
        Serial.println("Benutzerdefinierte Einstellungen werden gelesen");
        settings.loadDataFromPreferences();
    }
    
    /***************************************************************************
     * Initialisierung des WiFi Verbindung + NTP Server + Webserver für OTA
     **********************************************************************/
    if(settings.getWifiSettingsAvailable() == true)
    {
        wifi_connection_possible = settings.startWifi();
        if(wifi_connection_possible == true)
        {
            settings.startOTA();
            pZeit->NtpTimeUpdate(1.0, 1);
        }
    }
    else
    {
        Serial.println("Keine Zugangsdaten für WiFi hinterlegt");
    }
    
    /***************************************************************************
     * Initialisierung Bluetooth Kommunikation
     **************************************************************************/
    if (!SerialBT.begin("Wordclock_001"))
    {
        Serial.println("An error occurred initializing Bluetooth");
    }

    /***************************************************************************
     * Weitere Einstellungen und Initialisierungen
     **************************************************************************/
    pMuster = new Muster();
    pZeit = new Zeitmaster();
    pLedausgabe = new LED_Ausgabe((gpio_num_t)LED_PIN, 144);
    pLedausgabe->LedStartUp(settings.getStartPattern());
    
    
    
    /***********************************************************************
     * Initialisierung des Timers
     **********************************************************************/
    // Time 1: 1/(80MHZ/80) = 1us and count up
    timer1 = timerBegin(0, 80, true);
    timer2 = timerBegin(1, 80, true);
    // Attach onTimer function
    timerAttachInterrupt(timer1, &onTimer1, true);
    timerAttachInterrupt(timer2, &onTimer2, true);
    // Set alarm to 1000 ms and repeat it (true)
    timerAlarmWrite(timer1, 1*FACTOR_US_TO_S, true);
    timerAlarmWrite(timer2, NTP_TIMER_VALUE_SEC*FACTOR_US_TO_S, true);


    STARTINTERRUPT1;
    STARTINTERRUPT2;
    // Ende der Setup
    Serial.println("--- Setup beendet ---");
}

/*************************************************************************************************************************
 * 
**************************************************************************************************************************/
void loop()
{
    //Eventgetriggerte Ausgabe der Uhrzeit auf die LEDs (jede Sekunde)
    if (eventtrigger)
    {
        pZeit->printZeitmasterTime();
        eventtrigger = false;
        pMuster->setTimeMatrix(pMuster->getTimeMatrixFut(), pZeit->getHours(), pZeit->getMinutes());
        pMuster->setSimpleTimeNoEffects(pMuster->getTimeMatrixFut(), pMuster->getArbsMatrix(), settings.getColor());
        pLedausgabe->setPixelToColorMatrix(pMuster->getArbsMatrix());
    }
    
    
    //OTA und NTP Sync bei validen WiFi Daten
    if(settings.getWifiSettingsAvailable() == true)
    {
        if(WiFi.status() == WL_CONNECTED && wifi_connection_possible == true)
        {
            if(ntpSync)
            {
                ntpSync = false;
                pZeit->NtpTimeUpdate(1.0, 1);
            }
            settings.handleOTA();
        }
        else if(wifi_connection_possible == true) //einmaliges Verbinden beim Start war erfolgreich, aber Verbindung unterwegs verloren
        {
            settings.WifiAutoReconnect();
        }
    }

    
    // Empfange Befehle aus der App
    if (SerialBT.available())
    {
        appinterpreter.readCommandCharFromApp((char)SerialBT.read());
    }
}
