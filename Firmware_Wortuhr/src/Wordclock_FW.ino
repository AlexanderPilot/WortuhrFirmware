/*****************************************************************************************************************************************************************************************
 * Wordclock Software
 * created: 01.02.2019
 * by Alex P. and Vitali H.
 *****************************************************************************************************************************************************************************************/

/***************************************************************************
 * Einbinden von benötigten Bibliotheken
 **************************************************************************/
#include "Configurations.h"
#include "LED_Ausgabe.h"
#include "Muster.h"
#include "RTClib.h"

/***************************************************************************
 * Anlegen der Peripherie Instanzen
 **************************************************************************/
BluetoothSerial SerialBT;
Settings settings;
AppInterpreter appinterpreter;
LED_Ausgabe *pLedausgabe;
Zeitmaster *pZeit;
Muster myMuster;

/***************************************************************************
 * Timer; Eventrtigger
 * ************************************************************************/
hw_timer_t * timer = NULL;
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
    delay(100); Serial.println("--- Setup gestartet ---");

    /***********************************************************************
     * Initialisierung des Timers
     **********************************************************************/
    // Time 1: 1/(80MHZ/80) = 1us and count up
    timer = timerBegin(0, 80, true);
    // Attach onTimer function
    timerAttachInterrupt(timer, &onTimer, true);
    // Set alarm to 500 ms and repeat it (true)
    timerAlarmWrite(timer, 500000, true);
    // Start an alarm
    timerAlarmEnable(timer);
    
    /***************************************************************************
     * Weitere Einstellungen und Initialisierungen
     **************************************************************************/
    // Einstellung der Sprache fuer das Anzeigen der Zeit
    settings.setLanguage( GERMAN );

    // Start der Funktionen für die LED-Ausgabe
    pLedausgabe = new LED_Ausgabe((gpio_num_t)LED_PIN, 144);

    // Zeitfunktionen
    pZeit = new Zeitmaster();
    pZeit->setTimeDate(22,10,31,15,4,45); // ToDo: Sollte gelöscht werden, wenn die NTP Zeit bzw. App Zeiteinstellung funktioniert

    // Ender der Setup
    Serial.println("--- Setup beendet ---");
}

/*************************************************************************************************************************
 * 
**************************************************************************************************************************/
void loop()
{
    // Wird alle 500 ms getriggert in der ISR (siehe globale Einstellungen)
    if( eventtrigger )
    {
        // Update the clock
        myMuster.setTimeMatrix( myMuster.getTimeMatrixFut(), pZeit->getHours(), pZeit->getMinutes() );
        myMuster.setSimpleTimeNoEffects( myMuster.getTimeMatrixFut(), myMuster.getArbsMatrix(), {60,0,0} );
        pLedausgabe->setPixelToColorMatrix( myMuster.getArbsMatrix() );
        // reset trigger
        eventtrigger = false;
    }

    // Empfange Befehle aus der App
    if (SerialBT.available())
    {
        // ToDo: Erster Test wird werden, die Zeit ueber die App vorzugeben
        Serial.write(SerialBT.read());
    }
}
