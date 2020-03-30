/*
 * HSWITCH_config.c
 *
 *  Created on: Mar 10, 2020
 *      Author: madel
 */

#include "STD_TYPES.h"

#include "GPIO_interface.h"

#include "HSWITCH_interface.h"
#include "HSWITCH_config.h"



HSWITCH_PinsConfig_t HSWITCH_PinsConfig[SWITCHS_NUM] =
{
 {{PORTA , PIN0 , HSWITCH_MODE_PULLUP}} ,
// {{PORTA , PIN1 , HSWITCH_MODE_PULLUP}} ,
// {{PORTA , PIN2 , HSWITCH_MODE_FLOATING}}
};

