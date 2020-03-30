/*
 * HSWITCH_interface.h
 *
 *  Created on: Mar 9, 2020
 *      Author: madel
 */

#ifndef HSWITCH_INTERFACE_H_
#define HSWITCH_INTERFACE_H_

/* Switch_PINS (used only) in config.c */

#define SWITCH_1				(0)
#define SWITCH_2				(1)
#define SWITCH_3				(2)
#define SWITCH_4				(3)
#define SWITCH_5				(4)
#define SWITCH_6				(5)
#define SWITCH_7				(6)
#define SWITCH_8				(7)
#define SWITCH_9				(8)
#define SWITCH_10				(9)


#define SW_Pressed				(1)
#define SW_NotPressed			(0)


/* Here you can add some #define to make the pins has a unique name */
/* ex: #define SWITCH_Alarm		 SWITCH_1 */



/***************************************************Prototypes************************************************************/

ErrorStatus_t HSWITCH_Init(void);
ErrorStatus_t HSWITCH_ReadSwitchValue(uint8_t SWITCH_ID  , uint8_t* Value);

/* Runnable APIs */
uint8_t HSWITCH_GetSwitchState_Runnable(uint8_t SWITCH_ID);

#endif /* HSWITCH_INTERFACE_H_ */
