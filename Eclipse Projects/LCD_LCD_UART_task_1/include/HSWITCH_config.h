/*
 * HSWITCH_config.h
 *
 *  Created on: Mar 18, 2020
 *      Author: madel
 */

#ifndef HSWITCH_CONFIG_H_
#define HSWITCH_CONFIG_H_


/********************************************
 * To Change the number of SWITCHES used
 ********************************************/
#define SWITCHS_NUM			(1)

/********************************************
 * Select which micro-controller is working
 * Options:
 * 			1-AVR_MC
 * 			2-ARM_MC
 *
 ********************************************/
#define		ARM_MC









#ifdef AVR_MC

/* Direction options */
#define HSWITCH_MODE_FLOATING		MODE_INPUT_FLOATING
#define HSWITCH_MODE_PULLUP			MODE_INPUT_PULLUP

#endif

#ifdef ARM_MC

/* Direction options */
#define HSWITCH_MODE_FLOATING		MODE_INPUT_FLOATING
#define HSWITCH_MODE_PULLUP			MODE_INPUT_PULLUP
#define HSWITCH_MODE_PULLDOWN		MODE_INPUT_PULLDOWN

#endif

typedef struct{

	GPIO_PinConfig_t GPIO_PinConfig;

}HSWITCH_PinsConfig_t;

extern HSWITCH_PinsConfig_t HSWITCH_PinsConfig[];

#endif /* HSWITCH_CONFIG_H_ */
