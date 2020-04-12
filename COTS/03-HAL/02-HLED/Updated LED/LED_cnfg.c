/*
 * LED_cnfg.c
 *
 *  Created on: Feb 17, 2020
 *      Author:Mohamed Ibrahem 
 */

#include "GPIO_interface.h"
#include"LED.h"
#include"LED_cnfg.h"


const static LED_t LedArray[NUM_LEDS]=
{
		{.LED_Name=LED_Name1,LED1_Pin,LED1_Port,RESET,SET,LED1_MODE},
		{.LED_Name=LED_Name2,LED2_Pin,LED2_Port,SET,RESET,LED2_MODE},
		{.LED_Name=LED_Name3,LED3_Pin,LED3_Port,SET,RESET,LED3_MODE}
};




STD_ERROR LED_GetStruct(u32 CopyLed_Name,LED_t *CopyLed)
{
	STD_ERROR LocalError=NOK;
	for(u8 Counter=0;Counter<NUM_LEDS;Counter++)
	{
		if(CopyLed_Name==LedArray[Counter].LED_Name)
		{
			*CopyLed=LedArray[Counter];
			LocalError=OK;
		}
	}
	return LocalError;
}





inline STD_ERROR LED_GetPIN(void* PORT,u32 *PIN)
{
	STD_ERROR LocalError=NOK;
	*PIN=0;
	u32 LocalPort=(u32)PORT;

	for(u8 Counter=0;Counter<NUM_LEDS;Counter++)
	{
		if(LocalPort==(u32)LedArray[Counter].LED_Port)
		{
			*PIN |=LedArray[Counter].LED_Pin;
			LocalError=OK;
		}
	}
	return LocalError;
}
