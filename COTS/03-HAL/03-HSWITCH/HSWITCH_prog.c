/*
 * HSWITCH_prog.c
 *
 *  Created on: Mar 9, 2020
 *      Author: madel
 */

#include "STD_TYPES.h"

#include "GPIO_interface.h"

#include "HSWITCH_interface.h"
#include "HSWITCH_config.h"
#include "Scheduler_interface.h"


static uint8_t SWITCH_State[SWITCHS_NUM] ={SW_NotPressed};

void HSWITCH_Debouncing_Runnable(void);

/*================================================ Implementations of APIs ==================================================*/

ErrorStatus_t HSWITCH_Init(void)
{
	ErrorStatus_t LocalError = ERROR_OK;
	ErrorStatus_t LocalIterativeError = ERROR_OK;
	uint8_t Local_iterator_idx;

	for( Local_iterator_idx = 0 ; Local_iterator_idx < SWITCHS_NUM ; Local_iterator_idx++ )
	{
		LocalIterativeError = GPIO_InitStr( &(HSWITCH_PinsConfig[Local_iterator_idx].GPIO_PinConfig) );

		if(LocalIterativeError == ERROR_NOK)
		{
			LocalError = ERROR_NOK;
		}
	}

	return LocalError;
}
ErrorStatus_t HSWITCH_ReadSwitchValue(uint8_t SWITCH_ID  , uint8_t* Value)
{
	ErrorStatus_t LocalError = ERROR_OK;

	uint8_t LocalValue = LOW;
	LocalError = GPIO_GetPinValue(  &(HSWITCH_PinsConfig[SWITCH_ID].GPIO_PinConfig) , &LocalValue );

	if( HSWITCH_PinsConfig[SWITCH_ID].GPIO_PinConfig.MODE == HSWITCH_MODE_PULLUP)
	{
		*Value = LocalValue ^ 1;
	}
	else
	{
		*Value = LocalValue;
	}
	return LocalError;
}

uint8_t HSWITCH_GetSwitchState_Runnable(uint8_t SWITCH_ID)
{
	return SWITCH_State[SWITCH_ID];
}


/*================================================= Tasks =================================================*/

Task_t SWITCH_Debouncing_Task={

		.Runnable 	    = &HSWITCH_Debouncing_Runnable ,
		.Periodicity_ms = 20 						   ,
		.State			= TASK_RUNNING_STATE

};


void HSWITCH_Debouncing_Runnable(void)
{
	uint8_t SW_CurrentState = SW_NotPressed;

	static uint8_t SW_PrevState[SWITCHS_NUM]      = {SW_NotPressed};
	static uint8_t SW_PressedCounter[SWITCHS_NUM] = {0};

	for(uint8_t SW_index=0 ; SW_index<SWITCHS_NUM ; SW_index++)
	{
		HSWITCH_ReadSwitchValue(SW_index , &SW_CurrentState);
		if(SW_CurrentState == SW_PrevState[SW_index])
		{
			SW_PressedCounter[SW_index]++;
		}
		else
		{
			SW_PressedCounter[SW_index] = 0;
		}

		if(SW_PressedCounter[SW_index] == 5)
		{
			if( HSWITCH_PinsConfig[SW_index].GPIO_PinConfig.MODE == HSWITCH_MODE_PULLUP)
				{
					SWITCH_State[SW_index]     = SW_CurrentState ^ 1;
				}
				else
				{
					SWITCH_State[SW_index]     = SW_CurrentState;
				}
			SW_PressedCounter[SW_index]= 0;
		}
		else
		{
			/* Do Nothing */
		}
		SW_PrevState[SW_index] = SW_CurrentState;
	}
}
