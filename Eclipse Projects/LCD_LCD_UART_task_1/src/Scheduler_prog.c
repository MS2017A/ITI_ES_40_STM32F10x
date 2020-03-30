/*
 * Scheduler_prog.c
 *
 *  Created on: Mar 21, 2020
 *      Author: madel
 */
/*=================================================== #INCLUDES =======================================================*/
#include "STD_TYPES.h"

#include "STK_interface.h"
#include "STK_config.h"

#include "Scheduler_interface.h"
#include "Scheduler_config.h"

#include "RCC.h"

/*======================================== General Definitions & #Defines ============================================*/
typedef struct{

	SysTasks_t*	SysTask;
	uint32_t	RemainToExec;
	uint32_t	PeriodTicks;
	uint8_t		State;

}SystaskMainInfo_t;

SystaskMainInfo_t SystaskMainInfo[Tasks_Num];

/*======================================== Prototypes of Static Functions ==============================================*/
static void Scheduler_TickLoop(void);
static void Scheduler_SetOSFlag(void);

/*============================================= Implementations of APIs ==============================================*/

extern void Scheduler_Init(void)
{
	uint32_t index;
	uint32_t speed;
	for(index = 0 ; index<Tasks_Num ; index++)
	{
		SystaskMainInfo[index].SysTask 		= &SysTasks[index];
		SystaskMainInfo[index].RemainToExec =  SysTasks[index].First_Delay;
		SystaskMainInfo[index].PeriodTicks  =  SysTasks[index].Task->Periodicity_ms/TICK_MS;
		SystaskMainInfo[index].State	    =  SysTasks[index].Task->State;
	}
	/* Config The Systick Timer */
	/* Ibrahim Systick */
	STK_voidInit();
	STK_voidSetCallBack(&Scheduler_SetOSFlag);
	speed = RCC_GetAHP_Speed();
	STK_SetTimer_usec(TICK_MS,speed);

}



extern void Scheduler_Start(void)
{
	/* Start the timer */
	/* Ibrahim Systick */
	STK_voidStart();

	while(1)
	{
		if(OSTickFlag == 1)
		{
			OSTickFlag = 0;
			Scheduler_TickLoop();
		}
	}
}

extern uint8_t Scheduler_GetTick_ms(void)
{
	return	TICK_MS;
}



/*====================================== Implementation of Static Functions ========================================*/
static void Scheduler_TickLoop(void)
{
	uint8_t index;

	for(index = 0 ; index<Tasks_Num ; index++)
	{
		if(SystaskMainInfo[index].State == TASK_RUNNING_STATE)
		{
			if(SystaskMainInfo[index].RemainToExec == 0)
			{
				SystaskMainInfo[index].SysTask->Task->Runnable();
				SystaskMainInfo[index].RemainToExec = SystaskMainInfo[index].PeriodTicks;
			}

			SystaskMainInfo[index].RemainToExec--;
		}
	}
}

static void Scheduler_SetOSFlag(void)
{
	OSTickFlag = 1;
}
