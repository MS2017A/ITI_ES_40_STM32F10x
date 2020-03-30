/*
 * HLED_interface.h
 *
 *  Created on: Mar 9, 2020
 *      Author: madel
 */

#ifndef HLED_INTERFACE_H_
#define HLED_INTERFACE_H_


/* LED_PINS (used only) in config.c */

#define LED_1				(0)
#define LED_2				(1)
#define LED_3				(2)
#define LED_4				(3)
#define LED_5				(4)
#define LED_6				(5)
#define LED_7				(6)
#define LED_8				(7)
#define LED_9				(8)
#define LED_10				(9)


/* Here you can add some #define to make the pins has a unique name */
/* ex: #define LED_Alarm LED_1 */


/***************************************************Prototypes************************************************************/

ErrorStatus_t HLED_Init(void);
ErrorStatus_t HLED_WriteLEDValue(uint8_t LED_ID , uint8_t  Value);
ErrorStatus_t HLED_ReadLEDValue (uint8_t LED_ID  , uint8_t* Value);
ErrorStatus_t HLED_ToggleLEDValue (uint8_t LED_ID);

#endif
