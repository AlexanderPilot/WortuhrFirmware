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
word Matrix[12];

/***************************************************************************
 * Anlegen von globalen Variablen
 **************************************************************************/

byte ISRsec = 0;
byte ISRmin = 0;
byte ISRhour = 0;
bool showSerial = false;

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
    showSerial = true;
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
    //led_ausgabe.LedStartUp(settings.getStartPattern());
    //delay(2000);
    //led_ausgabe.setPixelToMatrix(0,0);
    //led_ausgabe.setPixelToMatrix(2);
    led_ausgabe.clearLEDs();
    //while(1);
        
    /****************************************
     * Initializierung Timer + Interrupt
     ****************************************/
    //Setzen des Timer0 mit Prescaler 80 --> 1us Taktung und Aufwärtszählung (true)
    timer = timerBegin(0, 80, true);
    //Interrupt an Timer0 koppeln
    timerAttachInterrupt(timer, &ISR_Timer, true);
    //Alarm wird jede Sekunde gesetzt, Alarm soll immer wieder wiederholt werden (true)
    timerAlarmWrite(timer, 1000*TIMER_VALUE_MS, true);
    // Start an alarm
    timerAlarmEnable(timer);
    
}

/****************************************************************************************************************************************************************************************************************************/

void loop()
{
    byte TestVar = 3;
    
    switch(TestVar)
    {
        case 0: //alle LEDs nacheinander einschalten
            for(int i = 0; i<=LED_NUM; i++)
            {
                led_ausgabe.setPixelToMatrix(i,30,30,30);
                Serial.print("LED Nummer");
                Serial.print(i);
                Serial.println("leuchtet");
                delay(50);
            }
            break;
        case 1: //yPos hochzählen während xPos gleich bleibt, dann nächste xPos
            for(int i=0; i<=NUM_COLUMN; i++) //
            {
                for(int j=0; j<=NUM_ROW; j++)
                {
                    led_ausgabe.setPixelToMatrix(i,j,30,30,30);
                    delay(100);
                }
            }
            break;
        case 2: //xPos hochzählen während yPos gleich bleibt, dann nächste yPos
            for(int j=0; j<=NUM_ROW; j++)
            {
                for(int i=0; i<=NUM_COLUMN; i++)
                {
                    led_ausgabe.setPixelToMatrix(i,j,30,30,30);
                    delay(100);
                }
            }
            break;
        case 3: //Ausgabe der Uhrzeit
            if(showSerial == true) //10ms Timer Interrupt
            {
                //Rendern der Ecken aus der Uhrzeit
                renderer.setCorners(ISRmin, 0, Matrix);

                //Rendern der Matrix aus der Uhrzeit 
                renderer.setTime(ISRhour, ISRmin, 0, Matrix);
                //Ausgabe der Matrix auf die LEDs
                led_ausgabe.setMatrixToLEDs(Matrix);
                Serial.print(ISRhour);
                Serial.print(":");
                Serial.print(ISRmin);
                Serial.print(":");
                Serial.println(ISRsec);
                showSerial = false;
            }
            break;
    }
}
