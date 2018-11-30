/*
 * Wordclock
 */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------
ToDo:
- Funktion um Structelemente in Textstring zu wandlen für Übergabe oder struct direkt an Funktion aus der Klasse DS3231 zu übergeben inkl. Vorbereitung der Klassenfunktion
- Struct global angelegt, kann aber auch in der klasse erfolgen aber lokale struct-Elemente in den RTOS Tasks (falls notwenidg)
- Funktion für die serielle Ausgabe der Zeit in einer Zeile --> keine mehrfachaufrufe zur Ausgabe mit allen
- renderer Anpassen, sodass struct übergeben werden kann und die Auswertung in der Funktion separat erfolgt
- Einbindung Bluetooth



------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Einbinden von benötigten Bibliotheken
#include "Configurations.h"

//Anlegen der RTOS Message Queues
xQueueHandle msgq_ntpTime;
xQueueHandle msgq_rtcTime;
xQueueHandle msgq_matrix;

//Anlegen der RTOS Semaphoren
SemaphoreHandle_t sema_ntp;
SemaphoreHandle_t sema_i2c;

//Anlegen der Peripherie Instanzen
WS2812 ledStrip = WS2812((gpio_num_t)LEDSTRIP_PIN,LED_NUM,0);
DS3231 ds3231(DS3231_ADDRESS);
TwoWire i2cRtc = TwoWire(I2C_CHANNEL);
hw_timer_t * timer = NULL;

struct myTime {
      uint16_t year;
      uint8_t month;
      uint8_t date;
      uint8_t dayOfWeek;
      uint8_t hour;
      uint8_t minute;
      uint8_t second;
};

uint32_t ISRcounter = 0;

void IRAM_ATTR ISR_Timer(){
      //ISR wird jede Sekunde ausgelöst. Eine ISR
      if(ISRcounter >= NTP_TIMER_VALUE_SEC){
            ISRcounter = 0;
            //Freigeben der Semaphore an den Task getNtp
            xSemaphoreGiveFromISR(sema_ntp, NULL);
      }
      ISRcounter++;
}

void printLocalTime()
{
      struct tm timeinfo;
      if(!getLocalTime(&timeinfo))
      {
            _DEBUG_PRINTLN("Failed to obtain time");
            return;
      }
      Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void getNtpTime(void *arg)
{
      //erzeugen eines lokalen Struct objects
      myTime ntpTime;
      //struct ntpTime anlegen um die Uhrzeit von ntp server auszulesen und das structin die msgq zu übergeben
      while (1)
      {
            if (xSemaphoreTake(sema_ntp, 1000)) //xSemaphoreTake(semaphore, time to wait for semaphore before going to blocked state)
            //semaphore wird in ISR freigegeben
            {
                  Serial.println("ISR ausgeloest");
                  _DEBUG_PRINTLN("Task getNtpTime receives NTP time");
                  //NTP Uhrzeit vom Server abfragen und in struct ntpTime speichern
                  ntpTime.year = 2018;
                  ntpTime.month = 12;
                  ntpTime.date = 1;
                  ntpTime.dayOfWeek = 2;
                  ntpTime.hour = 16;
                  ntpTime.minute = 35;
                  ntpTime.second = 17;
                  //Struct ntpTime in die Message Queue senden                  
                  if (xQueueSendToBack(msgq_ntpTime, &ntpTime, 500 / portTICK_RATE_MS) != pdTRUE)
                  {
                        _DEBUG_PRINTLN("Task getNtpTime failed to send value to queue ");
                  }
                  else
                  {
                        _DEBUG_PRINT("Task getNtpTime has send value to queue ");
                        //_DEBUG_PRINTLN(ntpTimeAsString);
                        _DEBUG_PRINTLN(ntpTime.year);
                        _DEBUG_PRINTLN(ntpTime.month);
                        _DEBUG_PRINTLN(ntpTime.date);
                        _DEBUG_PRINTLN(ntpTime.dayOfWeek);
                        _DEBUG_PRINTLN(ntpTime.hour);
                        _DEBUG_PRINTLN(ntpTime.minute);
                        _DEBUG_PRINTLN(ntpTime.second);
                  }
                  vTaskDelay(500 / portTICK_RATE_MS); // delay 500ms
            }
      }
}

void writeTimeToRtc(void *arg)
{
      bool error;
      struct myTime rtcTimeWrite;
      //struct rtcTime anlegen um den inhalt der msgq auszulesen
      while (1)
      {
            if (xQueueReceive(msgq_ntpTime, &rtcTimeWrite, 1000 / portTICK_RATE_MS) != pdTRUE)
            {
                  // max wait 1000ms
                  _DEBUG_PRINTLN("Task writeTimeToRtc fail to receive queued value");
            }
            else
            {
                  _DEBUG_PRINT("Task writeTimeToRtc received queued value ");
                  _DEBUG_PRINTLN(rtcTimeWrite.year);
                  _DEBUG_PRINTLN(rtcTimeWrite.month);
                  _DEBUG_PRINTLN(rtcTimeWrite.date);
                  _DEBUG_PRINTLN(rtcTimeWrite.dayOfWeek);
                  _DEBUG_PRINTLN(rtcTimeWrite.hour);
                  _DEBUG_PRINTLN(rtcTimeWrite.minute);
                  _DEBUG_PRINTLN(rtcTimeWrite.second);
                  _DEBUG_PRINTLN("Task writeTimeToRtc trying to access I2C bus");
                  if (xSemaphoreTake(sema_i2c, 1000)) //xSemaphoreTake(semaphore, time to wait for semaphore before going to blocked state)
                  {
                        _DEBUG_PRINTLN("Task writeTimeToRtc acces to I2C bus granted");
                        _DEBUG_PRINTLN("Task writeTimeToRtc start I2C communication");
                        i2cRtc.beginTransmission(DS3231_ADDRESS); //starting I2C communication to DS3231_ADRESS
                        //ds3231.writeTime(rtcTimeWrite);
                        //Funktion anpassen und per Übergabeparameter struct die Uhrzeit auf der RTC Speichern
                        _DEBUG_PRINTLN("Task writeTimeToRtc sending data");
                        i2cRtc.endTransmission(true); //stopping I2C communication
                        xSemaphoreGive(sema_i2c);
                  }
            }
            if (uxQueueMessagesWaiting(msgq_ntpTime) == 0)
            {
                  // no message? take a break
                  //vTaskDelay(500 / portTICK_RATE_MS); // delay 500ms
            }
      }
}

void readRtcTime(void *arg)
{
      struct myTime rtcTimeRead;
      while (1)
      {
            _DEBUG_PRINTLN("Task readRtcTime trying to get acces to I2C bus");
            vTaskDelay(10);
            if (xSemaphoreTake(sema_i2c, 1000)) //xSemaphoreTake(semaphore, time to wait for semaphore before going to blocked state)
            {
                  _DEBUG_PRINTLN("Task readRtcTime acces to I2C bus granted");
                  _DEBUG_PRINTLN("Task readRtcTime start I2C communication");
                  i2cRtc.beginTransmission(DS3231_ADDRESS); //starting I2C communication to DS3231_ADRESS
                  //rtcTime = ds3231.readTime();
                  //Rückgabeparameter ist die struct 
                  _DEBUG_PRINTLN("Task readRtcTime treading data");
                  i2cRtc.endTransmission(true); //stopping I2C communication
                  xSemaphoreGive(sema_i2c);
                  _DEBUG_PRINT("Task readRtcTime send the value ");
                  _DEBUG_PRINTLN(rtcTimeRead.year);
                  _DEBUG_PRINTLN(rtcTimeRead.month);
                  _DEBUG_PRINTLN(rtcTimeRead.date);
                  _DEBUG_PRINTLN(rtcTimeRead.dayOfWeek);
                  _DEBUG_PRINTLN(rtcTimeRead.hour);
                  _DEBUG_PRINTLN(rtcTimeRead.minute);
                  _DEBUG_PRINTLN(rtcTimeRead.second);
                  if (xQueueSendToBack(msgq_rtcTime, &rtcTimeRead, 500 / portTICK_RATE_MS) != pdTRUE)
                  {
                        _DEBUG_PRINTLN("Task readRtcTime failed to send value to queue ");
                  }
                  else
                  {
                        _DEBUG_PRINT("Task readRtcTime has send value to queue ");
                        _DEBUG_PRINT(rtcTimeRead.minute);
                  }
            }
            else
            {
                  _DEBUG_PRINTLN("Task readRtcTime acces to I2C bus not possible");
            }
            rtcTimeRead.minute++;
            vTaskDelay(500 / portTICK_RATE_MS); // delay 1000ms
      }
}

void renderRtcTime(void *arg)
{
      word MatrixRendered[11];
      
      
      
      struct myTime rtcTimeRead;
      //
      while (1)
      {
            if (xQueueReceive(msgq_rtcTime, &rtcTimeRead, 1000 / portTICK_RATE_MS) != pdTRUE)
            {
                  // max wait 1000ms
                  _DEBUG_PRINTLN("Task renderRtcTime fail to receive queued value");
            }
            else
            {
                  _DEBUG_PRINT("Task renderRtcTime get queued value ");
                  _DEBUG_PRINTLN(rtcTimeRead.minute);
                  
                  //rendern der Uhrzeit in Matrix Muster
                  //MatrixRendered = renderTime(rtcTimeRead);
                  MatrixRendered[0] |= 0b1111111111100000;
                  MatrixRendered[1] |= 0b1111111111100000;
                  
                  _DEBUG_PRINT("Task renderRtcTime send");
                  if (xQueueSendToBack(msgq_matrix, &MatrixRendered, 500 / portTICK_RATE_MS) != pdTRUE)
                  {
                        _DEBUG_PRINT("Task renderRtcTime failed to send value to queue ");
                  }
            }
            if (uxQueueMessagesWaiting(msgq_rtcTime) == 0)
            {
                  // no message? take a break
                  //vTaskDelay(500 / portTICK_RATE_MS); // delay 500ms
            }
            //vTaskDelay(500 / portTICK_RATE_MS); // delay 500ms
      }
}

void showMatrix(void *arg)
{
      word Matrix[11];
      //Matrix als Übergabeparameter aus der msgq, beinhaltet das LED Binärmuster
      while (1)
      {
            if (xQueueReceive(msgq_matrix, &Matrix, 1000 / portTICK_RATE_MS) != pdTRUE)
            {
                  // max wait 1000ms
                  _DEBUG_PRINTLN("Task showMatrix fail to receive queued value");
            }
            else
            {
                  _DEBUG_PRINT("Task showMatrix get queued value ");
                  _DEBUG_PRINTLN(Matrix[0]);
                  _DEBUG_PRINTLN(Matrix[1]);
                  //Ausgeben der Matrix über die LEDs
                  ledStrip.show();
            }
            if (uxQueueMessagesWaiting(msgq_matrix) == 0)
            {
                  // no message? take a break
                  //vTaskDelay(500 / portTICK_RATE_MS); // delay 500ms
            }
      }
}

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
            counterWiFiConnection++;
      }
      _DEBUG_PRINTLN("finished");
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("STA mode initialized");
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("receive NTP time for first time");
      configTime(gmtOffset_sec, daylightOffset_sec, NTP_SERVER_NAME);
      _DEBUG_PRINT(PRINT_SMALLTAB);
      printLocalTime();
      //---------------------------------------------------------------------------------
      //Initializierung von Bluetooth
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("starting bluetooth - not yet implemented");
      
      //---------------------------------------------------------------------------------
      //Initializierung des I2C-Bus
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("starting I2C - not yet implemented");
      i2cRtc.begin(SDA_PIN, SCL_PIN, I2C_FREQUENCY);
      
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
      //starting of other peripherals
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("starting other peripherals - not yet implemented");
       
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
      //Erzeugen der Message Queues
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN(PRINT_SEPARATOR);
      _DEBUG_PRINT(PRINT_SMALLTAB);
      _DEBUG_PRINTLN("creating message queues");
      msgq_ntpTime = xQueueCreate(1, sizeof(struct myTime));
      msgq_rtcTime = xQueueCreate(1, sizeof(struct myTime));
      msgq_matrix = xQueueCreate(8, sizeof(uint32_t));
       
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
      xTaskCreate(&writeTimeToRtc, "writeTimeToRtc", CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
      xTaskCreate(&readRtcTime, "readRtcTime", CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 1, NULL);
      xTaskCreate(&renderRtcTime, "renderRtcTime", CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 2, NULL);
      xTaskCreate(&showMatrix, "showMatrix", CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 3, NULL); 
}

void loop()
{
      delay(10);
}
