/*
 * Wordclock
 */

#include "Configurations.h"

xQueueHandle msgq_ntpTime;
xQueueHandle msgq_rtcTime;
xQueueHandle msgq_matrix;

SemaphoreHandle_t sema_1;
SemaphoreHandle_t sema_i2c;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    _DEBUG_PRINTLN("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


void getNtpTime(void *arg)
{
  uint32_t txpos1 = 0;
  while (1)
  {
    if (xSemaphoreTake(sema_1, 1000)) //xSemaphoreTake(semaphore, time to wait for semaphore before going to blocked state)
    //semaphore is released in ISR
    {
      
      _DEBUG_PRINTLN("getNtpTime send ");
      //get NTP Time
      
      //copy internal time in myTime struct for message queue
      printLocalTime();
      _DEBUG_PRINTLN(txpos1);
      if (xQueueSendToBack(msgq_ntpTime, &txpos1, 500 / portTICK_RATE_MS) != pdTRUE)
      {
        _DEBUG_PRINT("getNtpTime failed to send value to queue ");
        _DEBUG_PRINTLN(txpos1);
      }
      vTaskDelay(500 / portTICK_RATE_MS); // delay 500ms
      txpos1++;
    }
  }
}

void writeTimeToRtc(void *arg)
{
  uint32_t rxpos1;
  while (1)
  {
    if (xQueueReceive(msgq_ntpTime, &rxpos1, 1000 / portTICK_RATE_MS) != pdTRUE)
    {
      // max wait 1000ms
      _DEBUG_PRINTLN("writeTimeToRtc fail to receive queued value");
    }
    else
    {
      _DEBUG_PRINT("writeTimeToRtc get queued value ");
      _DEBUG_PRINTLN(rxpos1);
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
  uint32_t txpos2 = 0;
  while (1)
  {
    _DEBUG_PRINT("readRtcTime send ");
    _DEBUG_PRINTLN(txpos2);
    if (xQueueSendToBack(msgq_rtcTime, &txpos2, 500 / portTICK_RATE_MS) != pdTRUE)
    {
      _DEBUG_PRINT("readRtcTime failed to send value to queue ");
      _DEBUG_PRINTLN(txpos2);
    }
    vTaskDelay(500 / portTICK_RATE_MS); // delay 1000ms
    txpos2++;
  }
}

void renderRtcTime(void *arg)
{
  uint32_t txpos = 0;
  uint32_t rxpos = 0;
  while (1)
  {
    if (xQueueReceive(msgq_rtcTime, &rxpos, 1000 / portTICK_RATE_MS) != pdTRUE)
    {
      // max wait 1000ms
      _DEBUG_PRINTLN("renderRtcTime fail to receive queued value");
    }
    else
    {
      _DEBUG_PRINT("renderRtcTime get queued value ");
      _DEBUG_PRINTLN(rxpos);
      txpos = rxpos;
      _DEBUG_PRINT("renderRtcTime send");
      _DEBUG_PRINTLN(txpos);
      if (xQueueSendToBack(msgq_matrix, &txpos, 500 / portTICK_RATE_MS) != pdTRUE)
      {
        _DEBUG_PRINT("renderRtcTime failed to send value to queue ");
        _DEBUG_PRINTLN(txpos);
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
  uint32_t rxpos2;
  while (1)
  {
    if (xQueueReceive(msgq_matrix, &rxpos2, 1000 / portTICK_RATE_MS) != pdTRUE)
    {
      // max wait 1000ms
      _DEBUG_PRINTLN("showMatrix fail to receive queued value");
    }
    else
    {
      _DEBUG_PRINT("showMatrix get queued value ");
      _DEBUG_PRINTLN(rxpos2);
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
  // starting of serial communication
  Serial.begin(SERIAL_SPEED);
  _DEBUG_BEGIN(SERIAL_SPEED);
  delay(10);

  //---------------------------------------------------------------------------------
  // output of version number
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
  // initializing peripherals
  _DEBUG_PRINTLN("");
  _DEBUG_PRINTLN("Starting initialization of all peripherals ");

  //---------------------------------------------------------------------------------
  // starting WiFi in stationary modus
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
  //starting of bluetooth
  _DEBUG_PRINT(PRINT_SMALLTAB);
  _DEBUG_PRINTLN(PRINT_SEPARATOR);
  _DEBUG_PRINT(PRINT_SMALLTAB);
  _DEBUG_PRINTLN("starting bluetooth - not yet implemented");
  
  //---------------------------------------------------------------------------------
  //starting of I2C
  _DEBUG_PRINT(PRINT_SMALLTAB);
  _DEBUG_PRINTLN(PRINT_SEPARATOR);
  _DEBUG_PRINT(PRINT_SMALLTAB);
  _DEBUG_PRINTLN("starting I2C - not yet implemented");
  
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
  msgq_ntpTime = xQueueCreate(10, sizeof(uint32_t));
  msgq_rtcTime = xQueueCreate(10, sizeof(uint32_t));
  msgq_matrix = xQueueCreate(10, sizeof(uint32_t));
  
  //Erzeugen der Semaphoren
  _DEBUG_PRINT(PRINT_SMALLTAB);
  _DEBUG_PRINTLN(PRINT_SEPARATOR);
  _DEBUG_PRINT(PRINT_SMALLTAB);
  _DEBUG_PRINTLN("creating semaphores");
  sema_1 = xSemaphoreCreateMutex();
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
