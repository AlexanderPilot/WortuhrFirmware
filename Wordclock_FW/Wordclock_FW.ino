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

/***************************************************************************
 * Anlegen der RTOS Semaphoren
 **************************************************************************/
SemaphoreHandle_t sema_ntp;
SemaphoreHandle_t sema_i2c;

/***************************************************************************
 * Anlegen der Peripherie Instanzen
 **************************************************************************/
hw_timer_t * timer = NULL;
BluetoothSerial SerialBT;
Settings settings;


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

/***************************************************************************
 * RTOS Task für die Kommunikation mit der App über Bluetooth
 **************************************************************************/
void BluetoothConnection(void *arg)
{
    while (1)
    {
        
        vTaskDelay(10 / portTICK_RATE_MS); // delay 10ms
    }
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
    
    /***************************************************************************
     * Ausgabe der Versionsnummer
     **************************************************************************/
    Serial.println();
    Serial.println(PRINT_SEPARATOR_LONG);
    Serial.println();
    Serial.print("\tWordclock v");
    Serial.println(VERSION);
    Serial.println();
    Serial.println(PRINT_SEPARATOR_LONG);
    
    /** Lesen der Einstellungen aus dem EEPROM **/
    //settings.loadFromEEPROM();
    /***************************************************************************
     * Standardeinstellungen für Testzwecke, soll später aus dem EEPROM gelesen werden
     **************************************************************************/
    settings.setLanguage(LANGUAGE_DE_DE);
    
    settings.setWifiSSID("ASUS");
    settings.setWifiPW("Br8#Pojg56");
    //settings.setWifiSSID("UPC68EE18B");
    //settings.setWifiPW("Tw11tYbolz@#");
    //settings.setWifiSSID("Internet_MH");
    //settings.setWifiPW("WZ78AG27MGFF27DL");
    
    settings.setStartPattern(4);
    settings.setFadeMode(0);
    settings.setGmtTimeOffsetSec(3600);
    settings.setBrightnessPercent(100);
    settings.setCornerStartLed(0);
    settings.setColor(20, 20, 20);
    settings.setBrightnessPercent(100);
    
    //---------------------------------------------------------------------------------
    //Initializierung von Bluetooth
    _DEBUG_PRINTLN(PRINT_SEPARATOR);
    SerialBT.begin(BT_DEVICE_NAME); //Bluetooth device name
    _DEBUG_PRINTLN("Bluetooth gestartet");
    _DEBUG_PRINT("Geraetename: ");
    _DEBUG_PRINTLN(BT_DEVICE_NAME);
    //---------------------------------------------------------------------------------
    //Initializierung des EEPROMS
    _DEBUG_PRINTLN(PRINT_SEPARATOR);
    _DEBUG_PRINTLN("Initialisieren des EEPROM");
    EEPROM.begin(EEPROM_SIZE);
    //---------------------------------------------------------------------------------
    //definition of inouts / outputs
    _DEBUG_PRINTLN(PRINT_SEPARATOR);
    _DEBUG_PRINTLN("defining input / output - not yet implemented");
    //---------------------------------------------------------------------------------
    //Starten Timer und Interrupt

    //Setzen des Timer0 mit Prescaler 80 --> 1us Taktung und Aufwärtszählung (true)
    timer = timerBegin(0, 80, true);
    //Interrupt an Timer0 koppeln
    timerAttachInterrupt(timer, &ISR_Timer, true);
    //Alarm wird jede Sekunde gesetzt, Alarm soll immer wieder wiederholt werden (true)
    timerAlarmWrite(timer, 1000000, true);
    // Start an alarm
    timerAlarmEnable(timer);
    //---------------------------------------------------------------------------------
    //intialization of peripherals finished
    _DEBUG_PRINTLN(PRINT_SEPARATOR);
    _DEBUG_PRINTLN("initialization of peripherals finished");

    //---------------------------------------------------------------------------------
    _DEBUG_PRINTLN("");
    _DEBUG_PRINTLN("starting RTOS initialization");

}

/****************************************************************************************************************************************************************************************************************************/

void loop()
{
    if (Serial.available())
        {
            SerialBT.write(Serial.read());
        }
        if (SerialBT.available())
        {
            Serial.write(SerialBT.read());
        }
}
