#include <stdio.h>
#include <conio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

TaskHandle_t  myTask1Handle = NULL;

void myTask1(void* p) {
	int count = 0;
	while (1) {
		printf("Good morning everybody! : %d\r\n", count++);
		vTaskDelay(1000);
	}

}


int main_blinky()
{
	xTaskCreate(myTask1, "task1", 200, (void*)0, tskIDLE_PRIORITY, &myTask1Handle);


	vTaskStartScheduler();


	while (1)
	{

	}

}