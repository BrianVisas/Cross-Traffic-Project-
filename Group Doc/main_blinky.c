/*
 * FreeRTOS V202104.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* Standard includes. */
#include <stdio.h>
#include <conio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
//#include "timers.h"

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainQUEUE_RSU_TASK_PRIORITY		    ( tskIDLE_PRIORITY + 2 )

/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH					( 15 )

/*-----------------------------------------------------------*/

/*
 * Task processing data from the sensor and Road Side Unit 
 */
static void prvCarTask(void* pvParameters);

/*
 * Task emulates sensor operation
 */
static void prvSensorTask(void* pvParameters);

/*
 * Task emulates ifrasrtructure
 */
static void prvInfrastructureTask(void* pvParameters);

/*
 * Task emulates sensor RSU
 */
static void prvRsuTask(void* pvParameters);

/*-----------------------------------------------------------*/

/* Messaging queues */
static QueueHandle_t xRsuAskQueue = NULL;
static QueueHandle_t xRsuRespQueue = NULL;

static QueueHandle_t xSensorAskQueue = NULL;
static QueueHandle_t xSensorRespQueue = NULL;

static QueueHandle_t xInfrustructureAskQueue = NULL;
static QueueHandle_t xInfrustructureRespQueue = NULL;
/*-----------------------------------------------------------*/

typedef enum
{
	isAnyObjectPresent,
	noObject,
	objectDetected
}ask_msg_t;

typedef struct
{
	ask_msg_t msg;
	unsigned int distance;
}resp_msg_t;

typedef enum
{
	whatTrafficStatus,
	traffiClear,
	trafficCongested
}infrustructure_msg_t;

typedef enum
{
	sendLocation = 1
}rsu_ask_msg_t;

typedef struct
{
	unsigned distance;
	//unsigned l;
	// unsigned a;
}rsu_resp_msg_t;

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void main_blinky( void )
{

	xRsuAskQueue  = xQueueCreate(mainQUEUE_LENGTH, sizeof(rsu_ask_msg_t));
	xRsuRespQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(rsu_resp_msg_t));

	xSensorAskQueue  = xQueueCreate(mainQUEUE_LENGTH, sizeof(ask_msg_t));
	xSensorRespQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(resp_msg_t));

	xInfrustructureAskQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(infrustructure_msg_t));
	xInfrustructureRespQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(infrustructure_msg_t));

	xTaskCreate(prvCarTask,			            /* The function that implements the task. */
				"Car", 							/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 		/* The size of the stack to allocate to the task. */
				NULL, 							/* The parameter passed to the task - not used in this simple case. */
				mainQUEUE_RECEIVE_TASK_PRIORITY,/* The priority assigned to the task. */
				NULL);							/* The task handle is not required, so NULL is passed. */

	xTaskCreate(prvSensorTask,			        /* The function that implements the task. */
				"Sensor", 						/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 		/* The size of the stack to allocate to the task. */
				NULL, 							/* The parameter passed to the task - not used in this simple case. */
				mainQUEUE_RECEIVE_TASK_PRIORITY,/* The priority assigned to the task. */
				NULL);							/* The task handle is not required, so NULL is passed. */

	xTaskCreate(prvRsuTask,			            /* The function that implements the task. */
				"RSU", 						    /* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 		/* The size of the stack to allocate to the task. */
				NULL, 							/* The parameter passed to the task - not used in this simple case. */
		        mainQUEUE_RSU_TASK_PRIORITY,/* The priority assigned to the task. */
				NULL);							/* The task handle is not required, so NULL is passed. */

	xTaskCreate(prvInfrastructureTask,			/* The function that implements the task. */
				"infrastructure", 				/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 		/* The size of the stack to allocate to the task. */
				NULL, 							/* The parameter passed to the task - not used in this simple case. */
				mainQUEUE_RECEIVE_TASK_PRIORITY,/* The priority assigned to the task. */
				NULL);							/* The task handle is not required, so NULL is passed. */

	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/*
 * Task processing data from the sensor and Road Side Unit
 */
static void prvCarTask(void* pvParameters) 
{
	const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;
	const ask_msg_t ask = isAnyObjectPresent;
	const infrustructure_msg_t infr_ask = whatTrafficStatus;
	unsigned distance = 0;

	for (;; )
	{
		// ask sensor and rsu
		xQueueSend(xSensorAskQueue, &ask, 0U);
		xQueueSend(xInfrustructureAskQueue, &infr_ask, 0U);

		infrustructure_msg_t infr_resp;
		xQueueReceive(xInfrustructureRespQueue, &infr_resp, 0);

		resp_msg_t sensor_resp;
		xQueueReceive(xSensorRespQueue, &sensor_resp, portMAX_DELAY);

		if ((sensor_resp.msg == noObject) && (infr_resp == traffiClear) )
		{
			// run
			printf("Keep moving, no object detected and traffic clear\n");
			distance++;
		}
		else
		{
			// run and stop
			if (infr_resp == trafficCongested)
			{
				printf("Stop  traffic Congested \n");
			}

			if (sensor_resp.msg == objectDetected)
			{
				printf("Run %d m and stop, object was detected by Sensor \n", sensor_resp.distance);
				distance += sensor_resp.distance;
			}
		}

		rsu_ask_msg_t rsu_ask;
		xQueueReceive(xRsuAskQueue, &rsu_ask, 0);

		if (rsu_ask == sendLocation) 
		{
			rsu_resp_msg_t  resp;
			resp.distance = distance;
			xQueueSend(xRsuRespQueue, &resp, 0U);
		}

		/* Block for 5s. */
		vTaskDelay(xDelay);
	}
}
/*
 * Function that simulates the search for an object on the road
 */
resp_msg_t getSensorResponce(unsigned int count)
{
	resp_msg_t resp;

	if (count & 1)
	{
		resp.msg = objectDetected;
		resp.distance = 10;
	}
	else
	{
		resp.msg = noObject;
		resp.distance = 0;
	}

	return resp;
}

/*
 * Task emulates sensor operation
 */
static void prvSensorTask(void* pvParameters)
{
	ask_msg_t ask;
	resp_msg_t resp;
	const TickType_t xWait = portMAX_DELAY;

	unsigned int count = 0;
	for (;; )
	{
		xQueueReceive(xSensorAskQueue, &ask, xWait);

		if (ask == isAnyObjectPresent)
		{
			resp = getSensorResponce(count);

			xQueueSend(xSensorRespQueue, &resp, 0U);
		}

		count++;
	}
}

/*
 * Task emulates ifrasrtructure
 */
static void prvInfrastructureTask(void* pvParameters)
{
	ask_msg_t ask;
	infrustructure_msg_t resp;
	const TickType_t xWait = 4999 / portTICK_PERIOD_MS;

	unsigned int count = 0;
	for (;; )
	{
		xQueueReceive(xInfrustructureAskQueue, &ask, xWait);
		resp = ((count % 8) == 0) ? (trafficCongested) : (traffiClear);
		xQueueSend(xInfrustructureRespQueue, &resp, 0U);
		count++;
	}
}
/*-----------------------------------------------------------*/


/*
 * Task emulates RSU
 */
static void prvRsuTask(void* pvParameters)
{
	const rsu_ask_msg_t ask = sendLocation;
	rsu_resp_msg_t resp;
	const TickType_t xWait = portMAX_DELAY;
	const TickType_t xDelay = 10000 / portTICK_PERIOD_MS;

	for (;; ) 
	{
		xQueueSend(xRsuAskQueue, &ask, 0);
		xQueueReceive(xRsuRespQueue, &resp, xWait);

		printf("My location is %d m \n", resp.distance);

		/* Block for 10s. */
		vTaskDelay(xDelay);
	}
}