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
hw_timer_t * timer = NULL;
bool eventtrigger;
// ISR to set trigger
void IRAM_ATTR onTimer()
{
    eventtrigger = true;
}

// Start to delete after tests
pixel_t _color = {30,10,0} ;
// End to "delete"

/* START SETUP ************************************************************/
void setup()
{
    /***************************************************************************
     * Aufbauen der seriellen Kommunikation
     **************************************************************************/
    Serial.begin(SERIAL_SPEED);
    delay(100); Serial.println("--- Setup gestartet ---");
    
    /***************************************************************************
     * Weitere Einstellungen und Initialisierungen
     **************************************************************************/
    // Einstellung der Sprache fuer das Anzeigen der Zeit
    settings.setLanguage( GERMAN );

    // Start der Funktionen für die LED-Ausgabe
    pLedausgabe = new LED_Ausgabe((gpio_num_t)LED_PIN, 144);

    pMuster = new Muster();

    // Zeitfunktionen
    pZeit = new Zeitmaster();
    pZeit->setTimeDate(6,0,49,15,4,45); // ToDo: Sollte gelöscht werden, wenn die NTP Zeit bzw. App Zeiteinstellung funktioniert

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
    STARTINTERRUPT;

    // Ender der Setup
    Serial.println("--- Setup beendet ---");

}

/*************************************************************************************************************************
 * 
**************************************************************************************************************************/
void loop()
{
    // Wird jede 1s getriggert in der ISR (siehe globale Einstellungen)
    if( eventtrigger )
    {
        STOPINTERRUPT;
         
        // Update the clock
        pMuster->setTimeMatrix( pMuster->getTimeMatrixFut(), pZeit->getHours(), pZeit->getMinutes() );
        pMuster->setSimpleTimeNoEffects( pMuster->getTimeMatrixFut(), pMuster->getArbsMatrix(), _color );
        pLedausgabe->setPixelToColorMatrix( pMuster->getArbsMatrix() );
        
        // Verzoegerung evtl. nicht erforderlich
        // delay( 10 );
        
        // reset trigger
        eventtrigger = false;

        STARTINTERRUPT;
    }

    // Empfange Befehle aus der App
    //if ( SerialBT.available() )
    //{
    //    // ToDo: Erster Test wird werden, die Zeit ueber die App vorzugeben
    //    Serial.write(SerialBT.read());
    //}
    serialTestRead();
}
