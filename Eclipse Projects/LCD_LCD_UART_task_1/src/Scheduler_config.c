/*
 * Scheduler_config.c
 *
 *  Created on: Mar 21, 2020
 *      Author: madel
 */

#include "STD_TYPES.h"

#include "Scheduler_interface.h"
#include "Scheduler_config.h"

/*============================================= Private #defines ================================================*/
/* TODO: here make #defines of First Delay */






/* Here you add the name of the tasks here to be defined */
/*extern Task_t	LED_ON_Task;
extern Task_t	LED_OFF_Task;*/

//extern Task_t SWITCH_Debouncing_Task;
extern Task_t HLCD_Scheduler_Task;



/* place the task depends on their priority */
SysTasks_t	SysTasks[Tasks_Num] =
{
//	{ .Task = &LED_ON_Task  , .First_Delay = 0 }	,	//Priority - 1
//	{ .Task = &LED_OFF_Task , .First_Delay = 1 }		//Priority - 2
	{ .Task = &HLCD_Scheduler_Task  , .First_Delay = 0 }	,	//Priority - 1
};



