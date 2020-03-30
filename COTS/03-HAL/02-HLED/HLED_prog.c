/*
 * HLED_prog.c
 *
 *  Created on: Mar 9, 2020
 *      Author: madel
 */
#include "STD_TYPES.h"

#include "GPIO_interface.h"

#include "HLED_interface.h"
#include "HLED_config.h"


ErrorStatus_t HLED_Init()
{
	ErrorStatus_t LocalError = ERROR_OK;
	ErrorStatus_t LocalIterativeError_1 = ERROR_OK;
	ErrorStatus_t LocalIterativeError_2 = ERROR_OK;
	uint8_t Local_iterator_idx;

	for( Local_iterator_idx = 0 ; Local_iterator_idx < LEDS_NUM ; Local_iterator_idx++ )
	{
		LocalIterativeError_1 = GPIO_InitStr( &(HLED_PinsConfig[Local_iterator_idx].GPIO_PinConfig) );

		LocalIterativeError_2 = GPIO_SetPinValue( &(HLED_PinsConfig[Local_iterator_idx].GPIO_PinConfig) , (HLED_PinsConfig[Local_iterator_idx].LED_Init_Value) );

		if(LocalIterativeError_1 == ERROR_NOK || LocalIterativeError_2 == ERROR_NOK)
		{
			LocalError = ERROR_NOK;
		}
	}

	return LocalError;
}


ErrorStatus_t HLED_WriteLEDValue(uint8_t LED_ID , uint8_t Value)
{
	ErrorStatus_t LocalError = ERROR_OK;

	LocalError = GPIO_SetPinValue( &(HLED_PinsConfig[LED_ID].GPIO_PinConfig) , (Value^HLED_PinsConfig[LED_ID].LED_ACIVE_STATE) );

	return LocalError;
}


ErrorStatus_t HLED_ReadLEDValue (uint8_t LED_ID  , uint8_t* Value)
{
	ErrorStatus_t LocalError = ERROR_OK;

	uint8_t LocalValue = LOW;
	LocalError = GPIO_GetPinValue(  &(HLED_PinsConfig[LED_ID].GPIO_PinConfig) , &LocalValue );

	*Value = LocalValue ^HLED_PinsConfig[LED_ID].LED_ACIVE_STATE;

	return LocalError;
}

ErrorStatus_t HLED_ToggleLEDValue (uint8_t LED_ID)
{
	ErrorStatus_t LocalReadError = ERROR_OK;
	ErrorStatus_t LocalWriteError = ERROR_OK;
	ErrorStatus_t LocalTotalError = ERROR_OK;

	uint8_t LocalValue = LOW;


	LocalReadError =GPIO_GetPinValue(  &(HLED_PinsConfig[LED_ID].GPIO_PinConfig) , &LocalValue );

	LocalWriteError = GPIO_SetPinValue( &(HLED_PinsConfig[LED_ID].GPIO_PinConfig) , (LocalValue^1) );

	if( (LocalReadError == ERROR_OK) && (LocalWriteError == ERROR_OK) )
	{
		/* DO nothing */
	}
	else
	{
		LocalTotalError = ERROR_NOK;
	}

	return LocalTotalError;
}
