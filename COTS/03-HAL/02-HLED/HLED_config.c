/*
 * HLED_config.c
 *
 *  Created on: Mar 10, 2020
 *      Author: madel
 */
#include "STD_TYPES.h"

#include "GPIO_interface.h"
#include "HLED_interface.h"
#include "HLED_config.h"


/* Direction options */
#define HLED_ACTIVE_HIGH	(0)
#define HLED_ACTIVE_LOW		(1)

#ifdef AVR_MC

#define COMMON_LED_MODE		MODE_OUTPUT

#endif

#ifdef ARM_MC

#define COMMON_LED_MODE		MODE_OUTPUT_10_GP_PUSHPULL

#endif

HLED_PinsConfig_t HLED_PinsConfig[LEDS_NUM] =
/* PORT  | PIN  |    Direction     |   Active MODE    | Init Value*/
{
 {{PORTA , PIN0 , COMMON_LED_MODE} , HLED_ACTIVE_HIGH , LOW 	}, // LED_1
// {{PORTA , PIN1 , COMMON_LED_MODE} , HLED_ACTIVE_HIGH , HIGH	}, // LED_2
// {{PORTA , PIN2 , COMMON_LED_MODE} , HLED_ACTIVE_HIGH , LOW 	}  // LED_3
};
