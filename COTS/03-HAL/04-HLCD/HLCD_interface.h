/*
 * HLCD_interface.h
 *
 *  Created on: Mar 18, 2020
 *      Author: madel
 */

#ifndef HLCD_INTERFACE_H_
#define HLCD_INTERFACE_H_

#define FOUR_BITS_MODE		(7)
#define EIGHT_BITS_MODE		(11)

#define ONE_LINE_MODE		(0x00)
#define TWO_LINE_MODE		(0x08)

#define FONT_FIVE_x_SEVEN	(0x00)
#define FONT_FIVE_x_TEN		(0x04)

#define	BLINK_OFF			(0x00)
#define	BLINK_ON			(0x01)

#define CURSOR_OFF			(0x00)
#define CURSOR_ON			(0x02)

/* These Macros are used to define which x-axis line you want to access */
#define HLCD_FIRST_ROW		(0)
#define HLCD_SECOND_ROW		(1)



typedef uint8_t			HLCD_ErrorStatus_t;

#define 	HLCD_ERROR_OK				(0)
#define 	HLCD_ERROR_NOK				(1)
#define 	HLCD_ERROR_BUSY				(2)
#define 	HLCD_ERROR_NULL_POINTER		(4)


/*************************************************************************************************************/
/**********************************************Prototypes*****************************************************/
/*************************************************************************************************************/
void HLCD_Init(void);
void HLCD_WriteString(const char* Needed_String);
void HLCD_GoToXYPos(uint8_t XPos , uint8_t YPos);

/*************************************************************************************************************/
/**************************************Prototypes for Scheduler***********************************************/
/*************************************************************************************************************/
HLCD_ErrorStatus_t HLCD_WriteString_Runnable(const uint8_t* Needed_String , uint8_t Size);
HLCD_ErrorStatus_t HLCD_GoToXYPos_Runnable(uint8_t XPos , uint8_t YPos);
HLCD_ErrorStatus_t HLCD_ClearScreen_Runnable(void);
HLCD_ErrorStatus_t SetNotifyEndofJob_CBF_Runnable(void(*HLCD_NotifyEndofJob_CBF)(void));
HLCD_ErrorStatus_t HLCD_FreezeTheSystem(uint32_t Freeze_Time_ms);

/*============================ The Task of runnable that is going to be used =================================*/



#endif /* HLCD_INTERFACE_H_ */
