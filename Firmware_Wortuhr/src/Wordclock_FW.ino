/****************************************************************************************************************************************************************************************************************************
 * Wordclock Software
 * created: 01.02.2019
 ***************************************************************************************************************************************************************************************************************************/

/***************************************************************************
 * ToDo:
 * - Anpassen des Aufstartverhaltens (prüfen ob gültige Netzwerkinfo vorliegt, siehe setup-Schleife)
 * - Auslesen WLAN SSID und PW für Wifi.begin()
 * - Einbindung Bluetooth testen
 * - Ausgabe auf LEDs einbinden
 * - RTC auslesen nach der Synchronaisation führt zu Fehler in der ausgabe -->vermutlich auch im Bitmuster
 * 
 *
 *
**************************************************************************/

/***************************************************************************
 * Einbinden von benötigten Bibliotheken
 **************************************************************************/
#include "Configurations.h"
#include "LED_Ausgabe.h"
#include "Muster.h"

#define LED_PIN 26

/***************************************************************************
 * Anlegen der Peripherie Instanzen
 **************************************************************************/
hw_timer_t * timer = NULL;
BluetoothSerial SerialBT;
Settings settings;
AppInterpreter appinterpreter;


/***************************************************************************
 * Anlegen von globalen Variablen
 **************************************************************************/
//Für debug-Zwecke
uint32_t ISRcounter = 0;

/***************************************************************************
 * ISR für Timer Interrupt
 * Freigabe der RTOS Semaphore für NTP Abfrage
 **************************************************************************/
void IRAM_ATTR ISR_Timer()
{
    
}

/****************************************************************************************************************************************************************************************************************************/

void setup()
{
    /***************************************************************************
     * Anlegen von lokalen Variablen für setup
     **************************************************************************/
    bool WifiOK = true;
    uint8_t WifiTimeToConnect = 0;
  
    
    /***************************************************************************
     * Aufbauen der seriellen Kommunikation
     **************************************************************************/
    Serial.begin(SERIAL_SPEED);
    _DEBUG_BEGIN(SERIAL_SPEED);
    delay(10);
    
    /** Lesen der Einstellungen aus dem EEPROM **/
    //settings.loadFromEEPROM();
    /***************************************************************************
     * Standardeinstellungen für Testzwecke, soll später aus dem EEPROM gelesen werden
     **************************************************************************/
    settings.setLanguage(0);

    Serial.println("-------------------------------------------------------------------------------------------------------");
    
    /***************************************************************************
     * Ausgabe der Startsequenz & Test
     **************************************************************************/

    LED_Ausgabe mLedausgabe((gpio_num_t)LED_PIN, 144);
    mLedausgabe.setPixelTestT();
    Muster myMuster;

    int dir = 1;
    int lauf = 0;

    while( 1 )
    {
        myMuster.BuildMusterMatrix( myMuster.getArbsMatrix(), 3, lauf);
        mLedausgabe.setPixelToColorMatrix( myMuster.getArbsMatrix() );
        delay(100);
        lauf ++;
        if( lauf > 5 )
        {
            lauf = 0;
        }
    }
}

/****************************************************************************************************************************************************************************************************************************/

void loop()
{
    char AppBefehl[11];
    if (SerialBT.available())
    {
        Serial.write(SerialBT.read());
    }
}
