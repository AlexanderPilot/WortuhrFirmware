/*****************************************************************************************************************************************************************************************
 * Wordclock Software
 * created: 01.02.2019
 * by Alex P. and Vitali H.
 * 
 * 
 *****************************************************************************************************************************************************************************************/

/***************************************************************************
 * Einbinden von benötigten Bibliotheken
 **************************************************************************/
#include "Configurations.h"
#include "LED_Ausgabe.h"
#include "Muster.h"

#define STARTINTERRUPT (timerAlarmEnable(timer1))
#define STOPINTERRUPT (timerAlarmDisable(timer1))

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
bool eventtrigger;
bool ntpSync; //TODO: ISR für Synchronisation mit NTP Server
// ISR to set trigger
void IRAM_ATTR onTimer()
{
    eventtrigger = true;
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
    }
    else
    {
        Serial.println("Benutzerdefinierte Einstellungen werden gelesen");
        settings.loadDataFromPreferences();
    }
    settings.setColor(0,0,255);
    /***************************************************************************
     * Initialisierung des WiFi Verbindung + NTP Server + Webserver für OTA
     **********************************************************************/
    if(settings.getWifiSettingsAvailable() == true)
    {
        Serial.println("Initialisiere WiFi");
        settings.startWifi();
        //settings.startNtp();
        Serial.println("Initialisiere OTA Update");
        settings.startOTA();
        Serial.println("OTA Server abgeschlossen");
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
    // Start der Funktionen für die LED-Ausgabe
    pLedausgabe = new LED_Ausgabe((gpio_num_t)LED_PIN, 144);

    //Anzeige Startmuster
    pMuster = new Muster();

    // Zeitfunktionen
    pZeit = new Zeitmaster();
    pZeit->setTimeDate(6, 0, 49, 15, 4, 45); // ToDo: Sollte gelöscht werden, wenn die NTP Zeit bzw. App Zeiteinstellung funktioniert


    /***********************************************************************
     * Initialisierung des Timers
     **********************************************************************/
    // Time 1: 1/(80MHZ/80) = 1us and count up
    timer1 = timerBegin(0, 80, true);
    // Attach onTimer function
    timerAttachInterrupt(timer1, &onTimer, true);
    // Set alarm to 1000 ms and repeat it (true)
    timerAlarmWrite(timer1, 1*FACTOR_US_TO_S, true);
    STOPINTERRUPT;
    

    

    STARTINTERRUPT;
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
        eventtrigger = false;
        pMuster->setTimeMatrix(pMuster->getTimeMatrixFut(), pZeit->getHours(), pZeit->getMinutes());
        pMuster->setSimpleTimeNoEffects(pMuster->getTimeMatrixFut(), pMuster->getArbsMatrix(), settings.getColor());
        pLedausgabe->setPixelToColorMatrix(pMuster->getArbsMatrix());
        
    }
    
    //OTA und NTP Sync bei validen WiFi Daten
    if(settings.getWifiSettingsAvailable())
    {
        if(ntpSync)
        {
            ntpSync = false;
            settings.NtpTimeUpdate();
        }
        settings.handleOTA();
    }
    
    // Empfange Befehle aus der App
    if (SerialBT.available())
    {
        appinterpreter.readCommandCharFromApp((char)SerialBT.read());
    }
}
