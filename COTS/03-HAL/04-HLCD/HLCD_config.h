/*
 * HLCD_config.h
 *
 *  Created on: Mar 18, 2020
 *      Author: madel
 */

#ifndef HLCD_CONFIG_H_
#define HLCD_CONFIG_H_



typedef struct{

	GPIO_PinConfig_t GPIO_PinConfig;

}HLCD_PinsConfig_t;


extern HLCD_PinsConfig_t HLCD_PinsConfig[];


/******************************************************************************************
 ##########################################################################################
 ===================================LCD Configuration Options==============================
 ##########################################################################################
 *****************************************************************************************/


/********************************************
 * Select which micro-controller is working
 * Options:
 * 			1-AVR_MC
 * 			2-ARM_MC
 *
 ********************************************/
#define		ARM_MC




/********************************************
 * Select which MODE will be working
 * Options:
 * 			1-FOUR_BITS_MODE
 * 			2-EIGHT_BITS_MODE
 *
 ********************************************/
#define		LCD_MODE		FOUR_BITS_MODE




/********************************************
 * Operate the LCD with One line mode
 * or Two Line mode
 *
 * Options:
 * 			1- ONE_LINE_MODE
 * 			2- TWO_LINE_MODE
 *
 ********************************************/
#define LCD_NUM_LINE		TWO_LINE_MODE


/********************************************
 * Operate the LCD with which Size of Font
 *
 * Options:
 * 			1- FONT_FIVE_x_SEVEN
 * 			2- FONT_FIVE_x_TEN
 *
 ********************************************/
#define LCD_FONT			FONT_FIVE_x_TEN


/********************************************
 * Operate the LCD with BLinking Character
 * or not
 *
 * Options:
 * 			1- BLINK_ON
 * 			2- BLINK_OFF
 *
 ********************************************/
#define LCD_BLINK_MODE		BLINK_OFF


/********************************************
 * Operate the LCD with Cursor appearing
 * or not
 *
 * Options:
 * 			1- CURSOR_ON
 * 			2- CURSOR_OFF
 *
 ********************************************/
#define LCD_CURSOR_MODE		CURSOR_OFF




#endif /* HLCD_CONFIG_H_ */
