///* Funktionen RTOS Tasks */
//
///* Einbinden von Headerdateien */
//#include "RTOS_Tasks.h"
//
///* Funktionen */
//RTOS_Tasks::RTOS_Tasks()
//{
//  
//}
//
//void RTOS_Tasks::showMatrix(void *arg)
//{
//  uint32_t rxpos2;
//  while (1)
//  {
//    if (xQueueReceive(msgq_matrix, &rxpos2, 1000 / portTICK_RATE_MS) != pdTRUE)
//    {
//      // max wait 1000ms
//      _DEBUG_PRINTLN("showMatrix fail to receive queued value");
//    }
//    else
//    {
//      _DEBUG_PRINT("showMatrix get queued value ");
//      _DEBUG_PRINTLN(rxpos2);
//    }
//    if (uxQueueMessagesWaiting(msgq_matrix) == 0)
//    {
//      // no message? take a break
//      //vTaskDelay(500 / portTICK_RATE_MS); // delay 500ms
//    }
//  }
//}
