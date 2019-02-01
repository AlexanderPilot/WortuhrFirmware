/****************************************************************************************************************************************************************************************************************************
 * Wordclock Software - Branch für Test der LED Ausgabe
 * created: 01.02.2019
 ***************************************************************************************************************************************************************************************************************************/

/***************************************************************************
 * ToDo:
 * - Test der LED-Ausgabe (Renderer + Library)
 * 
**************************************************************************/


/***************************************************************************
 * Einbinden von benötigten Bibliotheken
 **************************************************************************/
#include "Configurations.h"

/***************************************************************************
 * Anlegen der Peripherie Instanzen
 **************************************************************************/
LED_Ausgabe led_ausgabe;
hw_timer_t * timer = NULL;
Renderer renderer;
Settings settings;

/***************************************************************************
 * Anlegen des Matrixspeichers
 **************************************************************************/
word Matrix[11];

/***************************************************************************
 * Anlegen von globalen Variablen
 **************************************************************************/

byte ISRsec = 0;
byte ISRmin = 0;
byte ISRhour = 0;

/****************************************************************************************************************************************************************************************************************************/

/***************************************************************************
 * ISR für Timer Interrupt
 * Hochzählen der Zeit für LED Test
 **************************************************************************/
void IRAM_ATTR ISR_Timer()
{
    //ISR wird über Timer jede Sekunde ausgelöst
    if(ISRsec >= 60)
    {
        ISRsec = 0;
        ISRmin++;
    }
    
    if(ISRmin >= 60)
    {
        ISRsec = 0;
        ISRmin = 0;
        ISRhour++;
    }
    
    if(ISRhour >=24)
    {
        ISRsec = 0;
        ISRmin = 0;
        ISRhour = 0;
    }
    
    ISRsec++; //Sekunden hochzählen
}

/****************************************************************************************************************************************************************************************************************************/

void setup()
{
    /***************************************************************************
     * Aufbauen der seriellen Kommunikation
     **************************************************************************/
    Serial.begin(SERIAL_SPEED);
    _DEBUG_BEGIN(SERIAL_SPEED);
    delay(10);
        
    /****************************************
     * Initializierung der LED Streifen
     ****************************************/
    _DEBUG_PRINTLN(PRINT_SEPARATOR);
    _DEBUG_PRINTLN("Initialisierung der LEDs");
    _DEBUG_PRINT("LED Anschlusspin: ");
    _DEBUG_PRINTLN(LEDSTRIP_PIN);
    randomSeed(micros());
    led_ausgabe.clearLEDs();
    _DEBUG_PRINT("Startmuster: ");
    _DEBUG_PRINTLN(settings.getStartPattern());
    led_ausgabe.LedStartUp(settings.getStartPattern());
    delay(2000);
    led_ausgabe.clearLEDs();
    led_ausgabe.setPixelToMatrix(0,0);
    led_ausgabe.setPixelToMatrix(2);
    //while(1);
        
    /****************************************
     * Initializierung Timer + Interrupt
     ****************************************/
    //Setzen des Timer0 mit Prescaler 80 --> 1us Taktung und Aufwärtszählung (true)
    timer = timerBegin(0, 80, true);
    //Interrupt an Timer0 koppeln
    timerAttachInterrupt(timer, &ISR_Timer, true);
    //Alarm wird jede Sekunde gesetzt, Alarm soll immer wieder wiederholt werden (true)
    timerAlarmWrite(timer, 1000000, true);
    // Start an alarm
    timerAlarmEnable(timer);
    
}

/****************************************************************************************************************************************************************************************************************************/

void loop()
{
    Serial.print("Uhrzeit: ");
    Serial.print(ISRhour);
    Serial.print(":");
    Serial.print(ISRmin);
    Serial.print(":");
    Serial.println(ISRsec);
    
    //Rendern der Ecken aus der Uhrzeit
    renderer.setCorners(ISRmin, 0, Matrix);

    //Rendern der Matrix aus der Uhrzeit 
    renderer.setTime(ISRhour, ISRmin, 0, Matrix);

    //Ausgabe der Matrix auf die LEDs
    led_ausgabe.setMatrixToLEDs(Matrix);
}
