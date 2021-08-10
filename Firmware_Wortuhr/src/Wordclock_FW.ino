/*****************************************************************************************************************************************************************************************
 * Wordclock Software
 * created: 01.02.2019
 * by Alex P. and Vitali H.
 * 
 * ToDos:
 *      1. App sende Zeit die am Handy angezeigt wird (Vitali)
 *      2. Lese per Bluetooth Zeit aus der App und stelle den DS3231 ein (Alex)
 * 
 *****************************************************************************************************************************************************************************************/

/***************************************************************************
 * Einbinden von benötigten Bibliotheken
 **************************************************************************/
#include "Configurations.h"
#include "LED_Ausgabe.h"
#include "Muster.h"
#include "RTClib.h"

#define STARTINTERRUPT (timerAlarmEnable(timer))
#define STOPINTERRUPT (timerAlarmDisable(timer))

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
hw_timer_t *timer = NULL;
bool eventtrigger;
// ISR to set trigger
void IRAM_ATTR onTimer()
{
    eventtrigger = true;
}

// Start to delete after tests
pixel_t _color = {10, 10, 10};
// End to "delete"

/* START SETUP ************************************************************/
void setup()
{
    /***************************************************************************
     * Aufbauen der seriellen Kommunikation
     **************************************************************************/
    Serial.begin(SERIAL_SPEED);
    delay(100);
    Serial.println("--- Setup gestartet ---");

    if (!SerialBT.begin("Wordclock_001"))
    {
        Serial.println("An error occurred initializing Bluetooth");
    }

    /***************************************************************************
     * Weitere Einstellungen und Initialisierungen
     **************************************************************************/
    // Einstellung der Sprache fuer das Anzeigen der Zeit
    settings.setLanguage(GERMAN);
    settings.setColor(30, 30, 30);

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
    timer = timerBegin(0, 80, true);
    // Attach onTimer function
    timerAttachInterrupt(timer, &onTimer, true);
    // Set alarm to 1000 ms and repeat it (true)
    timerAlarmWrite(timer, 1000000, true);
    // Start an alarm
    /*
    char SSID[] = {'0'};
    SSID[0] = 'O';
    SSID[1] = 'n';
    SSID[2] = 'L';
    SSID[3] = 'i';
    SSID[4] = 'n';
    SSID[5] = 'e';

    const char *ssid = (char *)SSID;
    const char *password = "Br8#Pojg56";
    WiFi.begin(ssid, password);
    unsigned long t_0;
    unsigned long t_last;
    Serial.println("Verbindungsversuch gestartet");
    while (WiFi.status() != WL_CONNECTED)
    {
        t_last = millis() - t_0;
        if (t_last > 5000)
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
    //const char *ssid = "OnLine";
    //    const char *ssid = "UPC68EE18B";
    //const char *password = settings.getWifiPW();
    unsigned long t_0;
    unsigned long t_last;

    // Wird jede 1s getriggert in der ISR (siehe globale Einstellungen)
    if (eventtrigger)
    {
        //pZeit->printZeitmasterTime();
        pMuster->setTimeMatrix(pMuster->getTimeMatrixFut(), pZeit->getHours(), pZeit->getMinutes());
        pMuster->setSimpleTimeNoEffects(pMuster->getTimeMatrixFut(), pMuster->getArbsMatrix(), settings.getColor());
        pLedausgabe->setPixelToColorMatrix(pMuster->getArbsMatrix());

        //Serielle Ausgabe
        //FIXME
        //pZeit->printZeitmasterTime();
        // reset trigger
        eventtrigger = false;

        //STARTINTERRUPT;

        //Serial.print(settings.getWifiSSID());
        //Serial.print(" ");
        //Serial.print(settings.getWifiPW());
        //Serial.println(" ");

        //Valide WiFi Daten verfügbar
        //if (settings.getWifiSettingsAvailable() == true)
        //{
        //    Serial.println("WiFi Daten verfügbar");
        /*WiFi.begin(settings.getWifiSSID(), settings.getWifiPW());
        while (WiFi.status() != WL_CONNECTED)
        {
            t_last = millis() - t_0;
            if (t_last > 5000)
            {
                Serial.println("Verbindungsversuch fehlgeschlagen");
                ESP.restart();
            }
        }
        Serial.println("Connected to network");*/
        //}
    }

    // Hier einlesen des Befehls
    // Empfange Befehle aus der App
    if (SerialBT.available())
    {
        appinterpreter.readCommandCharFromApp((char)SerialBT.read());
    }
}
