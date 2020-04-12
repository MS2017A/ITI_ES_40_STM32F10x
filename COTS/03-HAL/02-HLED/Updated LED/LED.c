#include"LED.h"
#include"LED_cnfg.h"



STD_ERROR LED_Init(u32 LED_Name)
{
	STD_ERROR LocalError=NOK;
	LED_t LocalLed;

	u32 PIN=0;
	LED_GetPIN(PORTA,&PIN);
	GPIO_InitPins(PORTA ,PIN ,LED_MODE);

	PIN=0;
	LED_GetPIN(PORTB,&PIN);
	GPIO_InitPins(PORTB ,PIN ,LED_MODE);

	PIN=0;
	LED_GetPIN(PORTC,&PIN);
	GPIO_InitPins(PORTC ,PIN ,LED_MODE);


	//	LocalError=LED_GetStruct(LED_Name,&LocalLed);
	//	if(LocalError==NOK)
	//		return NOK;
	//	GPIO_PinConfig_t GPIO_PinConfig={
	//			.PORT=LocalLed.LED_Port,
	//			.PIN=LocalLed.LED_Pin,
	//			.MODE=LocalLed.LED_MODE
	//	};
	//
	//
	//	//GPIO_InitPins(void* PORT , uint16_t P_NUMs , uint8_t MODE)
	//	GPIO_InitStr(&GPIO_PinConfig);
	//	return LocalError;
}

STD_ERROR LED_SetLED_ON(u32 LED_Name)
{
	STD_ERROR LocalError=OK;
	LED_t LocalLed;
	LocalError=LED_GetStruct(LED_Name,&LocalLed);
	if(LocalError==NOK)
		return NOK;

	GPIO_writePinValue(LocalLed.LED_Port ,LocalLed.LED_Pin , LocalLed.LED_ON);

	return LocalError;
}

STD_ERROR LED_SetLED_OFF(u32 LED_Name)
{
	STD_ERROR LocalError=OK;
	LED_t LocalLed;
		LocalError=LED_GetStruct(LED_Name,&LocalLed);
		if(LocalError==NOK)
			return NOK;
	//	GPIO_PinConfig_t GPIO_PinConfig={
	//			.PORT=LocalLed.LED_Port,
	//			.PIN=LocalLed.LED_Pin,
	//			.MODE=LocalLed.LED_MODE
	//	};
	//	LocalError=GPIO_SetPinValue(&GPIO_PinConfig ,LocalLed.LED_OFF);
		GPIO_writePinValue(LocalLed.LED_Port ,LocalLed.LED_Pin , LocalLed.LED_OFF);

		return LocalError;
}
