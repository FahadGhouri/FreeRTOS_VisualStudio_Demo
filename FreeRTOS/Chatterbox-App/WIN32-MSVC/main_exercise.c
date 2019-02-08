/*
 * FreeRTOS Kernel V10.1.1
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/******************************************************************************
 * NOTE: Windows will not be running the FreeRTOS project threads continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Windows port, or
 * this project application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Windows
 * port for further information:
 * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html
 *
 * NOTE 2:  This file only contains the source code that is specific to exercise 2
 * Generic functions, such FreeRTOS hook functions, are defined
 * in main.c.
 ******************************************************************************
 *
 * NOTE:  Console input and output relies on Windows system calls, which can
 * interfere with the execution of the FreeRTOS Windows port.  This demo only
 * uses Windows system call occasionally.  Heavier use of Windows system calls
 * can crash the port.
 */

/* Standard includes. */
#include <stdio.h>
#include <conio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* TODO: Priorities at which the tasks are created.
 */
#define TASK1_PRIORITY 1 
#define TASK2_PRIORITY 1
#define TASK3_PRIORITY 2   //Highest priority

/* TODO: output frequencey
 */
#define mainTASK_CHATTERBOX_OUTPUT_FREQUENCY_MS 1000

/*Task3 Handle
*/
TaskHandle_t xHandle = NULL;


/*-----------------------------------------------------------*/

/*
  * TODO: data structure
  */
struct taskData {
	char stringToPrint[10];
	uint8_t integerFlag;
}parametersToTask[3];


/*
 * TODO: C function (prototype) for task
 */
static void chatterBoxTask(void *pvParameters);

 

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void main_exercise( void )
{
/*
 * TODO: initialize data structures
 */
	strcpy(parametersToTask[0].stringToPrint,"Task1");
	strcpy(parametersToTask[1].stringToPrint, "Task2");
	strcpy(parametersToTask[2].stringToPrint, "Task3");

	parametersToTask[0].integerFlag = 0;    //Infinite task instance execution
	parametersToTask[1].integerFlag = 0;    //Infinite task instance execution
	parametersToTask[2].integerFlag = 1;    //Task instance execution 5 times

	/* 
	 * TODO: Create the task instances.
     */
	xTaskCreate(chatterBoxTask, "chatterbox", 500, &parametersToTask[0], TASK1_PRIORITY, NULL);
	xTaskCreate(chatterBoxTask, "chatterbox", 500, &parametersToTask[1], TASK2_PRIORITY, NULL);
	xTaskCreate(chatterBoxTask, "chatterbox", 500, &parametersToTask[2], TASK3_PRIORITY, &xHandle);
	
	 /*
	  * TODO: Start the task instances.
	  */
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
 * TODO: C function for tasks
 */

static void chatterBoxTask(void *pvParameters)
{
	struct taskData* temp = (struct taskData*) pvParameters;
	int count = 0;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = mainTASK_CHATTERBOX_OUTPUT_FREQUENCY_MS;

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		if (0 == temp->integerFlag)
		{
			printf("%s\n", temp->stringToPrint);
		}
		else if (1 == temp->integerFlag)
		{
			if (5 > count) {
				printf("%s\n", temp->stringToPrint);
				count++;
			}
			if (5 == count)
			{
				printf("Task 3 is terminated\n");

				// Use the handle to delete the task.
				if (xHandle != NULL)
				{
					vTaskDelete(xHandle);
				}
			}
		}
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

	}
}
