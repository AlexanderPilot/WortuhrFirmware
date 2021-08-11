/*****************************************************************************************************************************************************************************************
 * Wordclock Software
 * created: 01.02.2019
 * by Alex P. and Vitali H.
 * 
 * TODO:
 * Preferences Nutzung
 * Initialisierung WIFI mit SSID und PW aus WiFiControl 
 * 
 *****************************************************************************************************************************************************************************************/

/***************************************************************************
 * Einbinden von benötigten Bibliotheken
 **************************************************************************/
#include "Configurations.h"
#include "LED_Ausgabe.h"
#include "Muster.h"
#include "WiFiControl.h"

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
WiFiControl wificontrol;

/***************************************************************************
 * Timer; Eventrtigger
 * ************************************************************************/
hw_timer_t *timer1 = NULL;
bool eventtrigger;
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
    //Nur einmaligbenötigt, sodass Standardset in den Preferences hinterlegt ist
    //settings.writeDataToPreferences();
    settings.loadDataFromPreferences();
    
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

    /***************************************************************************
     * Laden der gespeicherten Einstellungen
     **************************************************************************/
    

    
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
    
    /***********************************************************************
     * Initialisierung des WiFi Verbindung
     **********************************************************************/
    
    //WiFi.begin(ssid_pointer, pw_pointer);
    
    //FIXME: mit ausgelesener SSID und PW führt es zu Speicher Übernutzung
    /*
    unsigned long t_0 = 0;
    unsigned long t_last = 0;
    Serial.println("Verbindungsversuch gestartet");
    while (WiFi.status() != WL_CONNECTED)
    {
        t_last = millis() - t_0;
        if (t_last > 500)
        {
            Serial.println("Verbindungsversuch fehlgeschlagen");
            ESP.restart();
        }
    }

    Serial.println("Connected to network");
    */

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
        //pZeit->printZeitmasterTime();
        pMuster->setTimeMatrix(pMuster->getTimeMatrixFut(), pZeit->getHours(), pZeit->getMinutes());
        pMuster->setSimpleTimeNoEffects(pMuster->getTimeMatrixFut(), pMuster->getArbsMatrix(), settings.getColor());
        pLedausgabe->setPixelToColorMatrix(pMuster->getArbsMatrix());
        eventtrigger = false;
    }

    //Auto-Reconnect to WiFi
//    wificontrol.autoReconnectWifi();

    // Empfange Befehle aus der App
    if (SerialBT.available())
    {
        appinterpreter.readCommandCharFromApp((char)SerialBT.read());
    }
}
