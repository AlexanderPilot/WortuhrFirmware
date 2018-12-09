/*
 * Wordclock
 */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------
ToDo:
- RTC läuft nicht weiter und Sekunden werden nicht hochgezählt --> Prüfen warum das so ist
- Einbindung Bluetooth testen

optional:
- Funktion für die serielle Ausgabe der Zeit in einer Zeile --> keine mehrfachaufrufe zur Ausgabe mit allen

------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Einbinden von benötigten Bibliotheken
#include "Configurations.h"

//Anlegen der RTOS Semaphoren
SemaphoreHandle_t sema_ntp;
SemaphoreHandle_t sema_i2c;

//Anlegen der Peripherie Instanzen
WS2812 ledStrip = WS2812((gpio_num_t)LEDSTRIP_PIN,LED_NUM,0);
DS3231 ds3231(DS3231_ADDRESS);
hw_timer_t * timer = NULL;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
BluetoothSerial SerialBT;
Renderer renderer;
Settings settings;

//Matrix zur Speicherung der Bitmuster für LED Ausgabe
word Matrix[11];

//Für debug-Zwecke
uint32_t ISRcounter = 0;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void IRAM_ATTR ISR_Timer(){
      //ISR wird über Timer jede Sekunde ausgelöst
      if(ISRcounter >= NTP_TIMER_VALUE_SEC)
      {
            ISRcounter = 0;
            //Freigeben der Semaphore an den Task getNtp
            xSemaphoreGiveFromISR(sema_ntp, NULL);
      }
      ISRcounter++;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void getNtpTime(void *arg)
{
      //Anlegen eines lokalen Structs
      struct myTime
      {
            uint16_t year;
            uint8_t month;
            uint8_t date;
            uint8_t dayOfWeek;
            uint8_t hour;
            uint8_t minute;
            uint8_t second;
      } ntpTime;
      
      while (1)
      {
            vTaskDelay(200);
            if (xSemaphoreTake(sema_ntp, 1000)) //xSemaphoreTake(semaphore, time to wait for semaphore before going to blocked state)
            //semaphore wird in ISR freigegeben
            {
                  //Update der NTP Zeit vom Server
                  timeClient.update();
                  
                  //Speichern der NTP Uhrzeit in lokalem Struct
                  /*
                  Die Funktionen getYear, getMonth, getDate sind nicht in der originalen Bibliothek enthalten und müssen noch implementiert werden um die vollständige Funktionalität darstellen zu können
                  */
                  ntpTime.year = 2018;
                  //ntpTime.year = timeClient.getYear();
                  ntpTime.month = 12;
                  //ntpTime.month = timeClient.getMonth();
                  ntpTime.date = 1;
                  //ntpTime.date = timeClient.getDate();
                  ntpTime.dayOfWeek = timeClient.getDay();
                  ntpTime.hour = timeClient.getHours();
                  ntpTime.minute = timeClient.getMinutes();
                  ntpTime.second = timeClient.getSeconds();

                  //Ausgabe der Uhrzeit
                  _DEBUG_PRINT("NTP Abfrage und Synchronisation");
                  //_DEBUG_PRINT(ntpTime.year);
                  //_DEBUG_PRINT("-");
                  //_DEBUG_PRINT(ntpTime.month);
                  //_DEBUG_PRINT("-");
                  //_DEBUG_PRINT(ntpTime.date);
                  //_DEBUG_PRINT("-");
                  //_DEBUG_PRINTLN(ntpTime.dayOfWeek);
                  _DEBUG_PRINT(ntpTime.hour);
                  _DEBUG_PRINT("-");
                  _DEBUG_PRINT(ntpTime.minute);
                  _DEBUG_PRINT("-");
                  _DEBUG_PRINTLN(ntpTime.second);

                  //Schreiben der Uhrzeit auf die RTC auf die klassen-internen Variablen (schreiben passiert hier noch nicht)
                  ds3231.setSeconds(ntpTime.second);
                  ds3231.setMinutes(ntpTime.minute);
                  ds3231.setHours(ntpTime.hour);
                  ds3231.setDayOfWeek(ntpTime.dayOfWeek);
                  ds3231.setDate(ntpTime.date);
                  ds3231.setMonth(ntpTime.month);
                  ds3231.setYear(ntpTime.year);
                  
                  //Semaphore für I2C Zugriff nehmen und Daten senden
                  if (xSemaphoreTake(sema_i2c, 100)) //xSemaphoreTake(semaphore, time to wait for semaphore before going to blocked state)
                  {
                        //schreiben der Uhrzeit zur RTC
                        ds3231.writeTime();
                        
                        //Zurückgabe der I2C Semaphore
                        xSemaphoreGive(sema_i2c);
                  }
                  
                  vTaskDelay(500 / portTICK_RATE_MS); // delay 500ms
            }
      }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void readRtcTime(void *arg)
{
      while (1)
      {
            if (xSemaphoreTake(sema_i2c, 1000)) //xSemaphoreTake(semaphore, time to wait for semaphore before going to blocked state)
            {
                  //Lesen der gespeicherten Zeit aus der RTC und speichern in klassen-internen Variablen
                  ds3231.readTime();
                  
                  //Rückgabe der I2C Semaphore
                  xSemaphoreGive(sema_i2c);
                  
                  _DEBUG_PRINTLN("RTC wird ausgelesen:");
                  //Serielle Ausgabe der ausgelesenen Uhrzeit 
                  _DEBUG_PRINT(ds3231.getYear());
                  _DEBUG_PRINT("-");
                  _DEBUG_PRINT(ds3231.getMonth());
                  _DEBUG_PRINT("-");
                  _DEBUG_PRINT(ds3231.getDate());
                  _DEBUG_PRINT("-");
                  _DEBUG_PRINTLN(ds3231.getDayOfWeek());
                  _DEBUG_PRINT(ds3231.getHours());
                  _DEBUG_PRINT("-");
                  _DEBUG_PRINT(ds3231.getMinutes());
                  _DEBUG_PRINT("-");
                  _DEBUG_PRINTLN(ds3231.getSeconds());
                  
                  //Rendern der Ecken aus der Uhrzeit
                  renderer.setCorners(ds3231.getMinutes(), settings.getCornersClockwise(), Matrix);

                  //Rendern der Matrix aus der Uhrzeit 
                  renderer.setTime(ds3231.getHours(), ds3231.getMinutes(), settings.getLanguage(), Matrix);

                  //Ausgabe der Matrix auf die LEDs
                  //led_ausgabe.setMatrixToLEDs(Matrix, true);
            }
            else
            {
                  //_DEBUG_PRINTLN("Task readRtcTime acces to I2C bus not possible");
            }
            vTaskDelay(500 / portTICK_RATE_MS); // delay 1000ms
      }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup()
{
      //---------------------------------------------------------------------------------
      //Initializierung der seriellen Kommunikation
      Serial.begin(SERIAL_SPEED);
      _DEBUG_BEGIN(SERIAL_SPEED);
      delay(10);
      
      //---------------------------------------------------------------------------------
      //Ausgabe der Version
      Serial.println();
      Serial.print(PRINT_SEPARATOR);
      Serial.println(PRINT_SEPARATOR);
      Serial.println();
      Serial.print("\tWordclock v");
      Serial.println(VERSION);
      Serial.println();
      Serial.print(PRINT_SEPARATOR);
      Serial.println(PRINT_SEPARATOR);
      //---------------------------------------------------------------------------------
      //Initializierung der Peripherie
      _DEBUG_PRINTLN("");
      _DEBUG_PRINTLN("Starte die Initialisierung der Peripherie");
      
      //---------------------------------------------------------------------------------
      //Initializierung WiFi im stationären Modus
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      WiFi.mode(WIFI_STA);
      WiFi.begin(STA_SSID, STA_PASSWORD);
      WiFi.setHostname("Name");
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("WiFi STA mode started");
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINT("Connecting to SSID: ");
      _DEBUG_PRINT(STA_SSID);
      
      while ((WiFi.status() != WL_CONNECTED))
      {
            delay(500);
            _DEBUG_PRINT(".");
            //counterWiFiConnection++;
      }
      _DEBUG_PRINTLN("finished");
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("STA mode initialized");
      //---------------------------------------------------------------------------------
      //Initializierung von Bluetooth
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("Starte Bluetooth Initialisierung");
      SerialBT.begin("Wordclock"); //Bluetooth device name
      _DEBUG_PRINTLN("Bluetooth gestartet");
      
      //---------------------------------------------------------------------------------
      //Initializierung des I2C-Bus
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);      
      Wire.begin(SDA_PIN, SCL_PIN);
      
      //---------------------------------------------------------------------------------
      //Initializierung des LED Strip
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("starting LED strip");
      _DEBUG_PRINT("starting pattern ");
      ledStrip.setColorOrder("GRB");
      _DEBUG_PRINT("Output of starting pattern version ");
      _DEBUG_PRINTLN(START_PATTERN);
      ledStrip.startPattern(START_PATTERN);
      //---------------------------------------------------------------------------------
      //Starten NTP Server
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("Starten des NTP Servers");
      timeClient.begin();
      timeClient.setTimeOffset(gmtOffset_sec);
      //---------------------------------------------------------------------------------
      //definition of inouts / outputs
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);
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
      //intialization of perifpherals finished
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINTLN("initialization of peripherals finished");
      
      //---------------------------------------------------------------------------------
      _DEBUG_PRINTLN("");
      _DEBUG_PRINTLN("starting RTOS initialization");
        
      //---------------------------------------------------------------------------------
      //Erzeugen der Semaphoren
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("creating semaphores");
      sema_ntp = xSemaphoreCreateMutex();
      sema_i2c = xSemaphoreCreateMutex();
      
      //Erzeugen der Tasks
      /*
      Prioritäten: je größer die Nummer, desto größer die Priorität
      */
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("create RTOS tasks");
      xTaskCreate(&getNtpTime, "getNtpTime", CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 4, NULL);
      xTaskCreate(&readRtcTime, "readRtcTime", CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 1, NULL);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
