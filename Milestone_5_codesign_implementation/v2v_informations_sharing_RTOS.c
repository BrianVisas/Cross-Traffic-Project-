#include <stdio.h>
#include <conio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

QueueHandle_t qh = 0;

TaskHandle_t mytaskhendler = NULL;
TaskHandle_t mytaskhendler1 = NULL;
void collect(void* P)
{
    
    while (1)
    {

        printf("Information collected sucessfully from vehicle B\r\n");
        vTaskDelay(500);
       
       vTaskDelete(mytaskhendler);
    }
    fflush(stdout);
}

void task_tx(void* p)
{
    int myInt = 2;
    while (1)
    {
        
        if (!xQueueSend(qh, &myInt, 500)) {
            puts("Failed to send collected information to cloud with 500ms");
            printf("Vehicle A send Information to cloud: %u\n", myInt);
        }
        else
        {
            printf("Vehicle A send Information to cloud: %u\n", myInt);
        }
        }
        vTaskDelay(1000);
        vTaskDelete(qh);
    }


void task_rx(void* p)
{
    int myInt = 0;
    while (1)
    {
        if (!xQueueReceive(qh, &myInt, 1000)) {
            puts("Failed to receive item within 1000 ms");
        }
        else {
            printf("Information successfully Received by cloud: %u\n", myInt);
        }
    }
    vTaskDelete(qh);
}
void task_tx_b(void* p)
{
    int myInt = 4;
    while (1)
    {
        myInt++;
        if (!xQueueSend(qh, &myInt, 2000)) {
            puts("Failed to send collected information to cloud with 500ms");
        }
        else {
            printf("Information successfully broadcasted from cloud: %u\n", myInt);
        }
        vTaskDelay(4000);
        vTaskDelete(qh);
    }
}

void task_rx_b(void* p)
{
    int myInt = 0;
    while (1)
    {
        if (!xQueueReceive(qh, &myInt, 4000)) {
            puts("Failed to receive item within 4000 ms");
        }
        else {
            printf("Information successfully Received by vehicle B: %u\n", myInt);
        }
    }
    vTaskDelete(qh);
}
int main_blinky()
{
    int pass = 50;
    qh = xQueueCreate(1, sizeof(int));
    xTaskCreate(collect, "collect", 200, (void*)pass, 1, &mytaskhendler);
    xTaskCreate(task_tx, (signed char*)"t1", 1024, 0, 1, 0);
    xTaskCreate(task_rx, (signed char*)"t2", 1024, 0, 1, 0);
    xTaskCreate(task_tx_b, (signed char*)"t1", 1024, 0, 1, 0);
    xTaskCreate(task_rx_b, (signed char*)"t2", 1024, 0, 1, 0);
    vTaskStartScheduler();

    return -1;
}
