/*
 * HLCD_config.c
 *
 *  Created on: Mar 18, 2020
 *      Author: madel
 */
#include "STD_TYPES.h"

#include "GPIO_interface.h"

#include "HLCD_interface.h"
#include "HLCD_config.h"





#ifdef AVR_MC

	#define LCD_PIN_MODE	MODE_OUTPUT

#endif

#ifdef ARM_MC

	#define LCD_PIN_MODE	MODE_OUTPUT_10_GP_PUSHPULL

#endif

#if LCD_MODE == EIGHT_BITS_MODE

HLCD_PinsConfig_t HLCD_PinsConfig[LCD_MODE] =
{

{{PORTA , PIN0 , LCD_PIN_MODE}} , //RS
{{PORTA , PIN1  , LCD_PIN_MODE}} , //RW
{{PORTA , PIN2  , LCD_PIN_MODE}} , //EN

{{PORTA , PIN3 , LCD_PIN_MODE}} , //DO
{{PORTA , PIN4 , LCD_PIN_MODE}} , //D1
{{PORTA , PIN5 , LCD_PIN_MODE}} , //D2
{{PORTA , PIN6 , LCD_PIN_MODE}} , //D3
{{PORTA , PIN11 , LCD_PIN_MODE}} , //D4
{{PORTA , PIN10 , LCD_PIN_MODE}} , //D5
{{PORTA , PIN9 , LCD_PIN_MODE}} , //D6
{{PORTA , PIN8 , LCD_PIN_MODE}}   //D7

};

#elif	LCD_MODE == FOUR_BITS_MODE

HLCD_PinsConfig_t HLCD_PinsConfig[LCD_MODE] =
{

{{PORTA , PIN0 , LCD_PIN_MODE}} , //RS
{{PORTA , PIN1 , LCD_PIN_MODE}} , //RW
{{PORTA , PIN2 , LCD_PIN_MODE}} , //EN

{{PORTA , PIN11 , LCD_PIN_MODE}} , //D4
{{PORTA , PIN10 , LCD_PIN_MODE}} , //D5
{{PORTA , PIN9 , LCD_PIN_MODE}} , //D6
{{PORTA , PIN8 , LCD_PIN_MODE}}   //D7

};

#endif
