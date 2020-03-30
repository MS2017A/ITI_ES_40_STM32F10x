/*
 * HLED_config.h
 *
 *  Created on: Mar 14, 2020
 *      Author: madel
 */

#ifndef HLED_CONFIG_H_
#define HLED_CONFIG_H_


/********************************************
 * To Change the number of LEDS used
 ********************************************/
#define LEDS_NUM			(1)



/********************************************
 * Select which micro-controller is working
 * Options:
 * 			1-AVR_MC
 * 			2-ARM_MC
 *
 ********************************************/
#define		ARM_MC







typedef struct{

	GPIO_PinConfig_t GPIO_PinConfig;
	uint8_t LED_ACIVE_STATE;
	uint8_t LED_Init_Value;

}HLED_PinsConfig_t;


extern HLED_PinsConfig_t HLED_PinsConfig[];



#endif /* HLED_CONFIG_H_ */
