/*
 * tasks.c
 *
 *  Created on: Apr 5, 2020
 *      Author: Mahmoud Gamal & Mohamed Anis
 */

#include "STD_TYPES.h"
#include "uart_interface.h"
#include "HSWITCH_interface.h"
#include "Scheduler_interface.h"
#include "task.h"

/*****************
 *
 *
 * WHY SHOULDN'T WE MAKE AN EXTERN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *
 * *****************************/


#define NUM_OF_SWITCHES		6
#define EDIT_SWITCH			SWITCH_1
#define UP_SWITCH			SWITCH_2
#define DOWN_SWITCH			SWITCH_3
#define RIGHT_SWITCH		SWITCH_4
#define LEFT_SWITCH			SWITCH_5
#define DATE_SWITCH			SWITCH_6
#define NO_SWITCH_PRESSED	0XFF

#define DISPLAY_TIME				0
#define DISPLAY_DATE				1

#define ENABLED						0
#define DISABLED					1

#define HLCD_FLAG_RAISED			0
#define HLCD_FLAG_LOWERED			1


#define HLCD_CLEAR_STATE		1
#define HLCD_GOTO_STATE			2
#define HLCD_DISPLAY_STATE		3

typedef struct{
	u8 hoursTens;
	u8 hoursUnits;

	u8 colon1;

	u8 minutesTens;
	u8 minutesUnits;

	u8 colon2;

	u8 secondsTens;
	u8 secondsUnits;

	u8 cursorPosition;
}time_t;

static void HLCD_algorithm(void);


static time_t Time =
{
		.hoursTens = '0',
		.hoursUnits = '0',

		.colon1 = ':',

		.minutesTens = '0',
		.minutesUnits = '0',

		.colon2 = ':',

		.secondsTens = '0',
		.secondsUnits = '0',

		.cursorPosition = 4

},PrintTime;
u8 displayStatus = DISPLAY_TIME;

static u8 RxData[NUM_OF_SWITCHES] = {0};

static u8 RxFlag;

const Task_t TASK_ChangeClock = {
		.Runnable = RUNNABLE_ChangeClock,
		.Periodicity_ms = 5
};

/***************************************************************
 *
 *
 * ASK TOURKEYYYYYYYYYYYYY
 *
 *			we are trying to
 *
 * static const u16 ONE_SECOND = (u16)1000/TASK_ChangeClock.Periodicity_ms;
 *
 *
 *
 *
 *
 *
 *
 * *****************************************************/

static u8 HLCD_flag = HLCD_FLAG_LOWERED;
void RUNNABLE_sendSwitchStatus(void)
{
	u8 switchsStatus[NUM_OF_SWITCHES]={0};

	for (u8 i = 0; i < NUM_OF_SWITCHES ; i++)
	{
		switchsStatus[i] = i<<1;
	}

	for (u8 i = 0; i < NUM_OF_SWITCHES ; i++)
	{
		switchsStatus[i] |= HSWITCH_GetSwitchState(i);
	}

	UART_errTransmit(switchsStatus,NUM_OF_SWITCHES);
}




void RUNNABLE_ChangeClock(void)
{
	const u16 ONE_SECOND = (u16)1000/TASK_ChangeClock.Periodicity_ms;
	static time_t TempTime, BackgroundTime;
	static u8 enableEdit = DISABLED;
	if (RxFlag == 1)
	{
		u8 index=0;

		static u8 	requiredSwitch = NO_SWITCH_PRESSED ;
		static u8	lastSwitch = 0;

		u8 *clockPointer;
		clockPointer = (u8*)&BackgroundTime;
		RxFlag = 0;

		for(index = 0; index < NUM_OF_SWITCHES; index ++)
		{

			if((RxData[index]&1) == SW_Pressed)
			{
				requiredSwitch = (RxData[index]>>1);
				break;
			}
			else requiredSwitch = NO_SWITCH_PRESSED;

		}

		if(lastSwitch != requiredSwitch)
		{
			lastSwitch = requiredSwitch;
			switch(requiredSwitch)
			{
			case EDIT_SWITCH:
				enableEdit ^= 1;
				displayStatus = DISPLAY_TIME;
				if (enableEdit == ENABLED)
				{
					Time.cursorPosition = 0;
					BackgroundTime = Time;
				}
				else if (enableEdit == DISABLED)				//00:00:00
				{
					if((Time.cursorPosition == 4) || (Time.cursorPosition == 5))
					{
						Time.hoursTens = BackgroundTime.hoursTens;
						Time.hoursUnits = BackgroundTime.hoursUnits;
					}
					else if((Time.cursorPosition == 7) || (Time.cursorPosition == 8))
					{
						Time.minutesTens = BackgroundTime.minutesTens;
						Time.minutesUnits = BackgroundTime.minutesUnits;
					}
					else if((Time.cursorPosition == 4) || (Time.cursorPosition == 5))
					{
						Time.secondsTens = BackgroundTime.secondsTens;
						Time.secondsUnits = BackgroundTime.secondsUnits;
					}
				}
				break;
			case UP_SWITCH:
				if(enableEdit == ENABLED)
				{
					clockPointer[(BackgroundTime.cursorPosition-4)]++;
					switch(BackgroundTime.cursorPosition)
					{
					case 4:
						if(BackgroundTime.hoursTens == '3')
							BackgroundTime.hoursTens='0';
						break;
					case 5:
						if(((BackgroundTime.hoursUnits == '4') && (BackgroundTime.hoursTens == '2')) || (BackgroundTime.hoursUnits > '9'))
							BackgroundTime.hoursTens='0';
						break;
					case 7:
						if(BackgroundTime.minutesTens == '6')
							BackgroundTime.minutesTens='0';
						break;
					case 8:
						if(BackgroundTime.minutesUnits > '9')
							BackgroundTime.minutesUnits='0';
						break;
					case 10:
						if(BackgroundTime.secondsTens == '6')
							BackgroundTime.secondsTens='0';
						break;
					case 11:
						if(BackgroundTime.secondsUnits > '9')
							BackgroundTime.secondsUnits='0';
						break;
					}
				}
				break;

			case DOWN_SWITCH:
				if(enableEdit == ENABLED)
				{
					clockPointer[(BackgroundTime.cursorPosition-4)]--;

					switch(BackgroundTime.cursorPosition)
					{
					case 4:
						if(((BackgroundTime.hoursTens < '0') && (BackgroundTime.hoursUnits < '4')))
							BackgroundTime.hoursTens= '2';
						else if (BackgroundTime.hoursTens < '0')
						{
							BackgroundTime.hoursTens= '1';
						}
						break;
					case 5:
						if((BackgroundTime.hoursTens == '2') &&  (BackgroundTime.hoursUnits < '0'))
							BackgroundTime.hoursUnits='3';

						else if((BackgroundTime.hoursTens < '2')&&  (BackgroundTime.hoursUnits < '0'))
							BackgroundTime.hoursUnits='9';
						break;
					case 7:
						if(BackgroundTime.minutesTens < '0')
							BackgroundTime.minutesTens='5';
						break;
					case 8:
						if(BackgroundTime.minutesUnits < '0')
							BackgroundTime.minutesUnits='9';
						break;
					case 10:
						if(BackgroundTime.secondsTens < '0')
							BackgroundTime.secondsTens='5';
						break;
					case 11:
						if(BackgroundTime.secondsUnits < '0')
							BackgroundTime.secondsUnits='9';
						break;

					}
				}
				break;

			case LEFT_SWITCH:
				if(enableEdit == ENABLED)
				{
					Time.cursorPosition--;

					if(Time.cursorPosition == 3)
					{
						Time.cursorPosition = 11;
						Time.hoursTens = BackgroundTime.hoursTens;
						Time.hoursUnits = BackgroundTime.hoursUnits;
					}
					else if(Time.cursorPosition == 6)
					{
						Time.cursorPosition = 5;
						Time.minutesTens = BackgroundTime.minutesUnits;
						Time.minutesUnits = BackgroundTime.minutesUnits;
					}
					else if(Time.cursorPosition == 9)
					{
						Time.cursorPosition = 8;
						Time.secondsTens = BackgroundTime.secondsTens;
						Time.secondsUnits = BackgroundTime.secondsUnits;
					}

					BackgroundTime = Time;
				}
				break;

			case RIGHT_SWITCH:
				if(enableEdit == ENABLED)
				{

					Time.cursorPosition++;

					if(Time.cursorPosition == 12)
					{
						Time.cursorPosition = 4;
						Time.secondsTens = BackgroundTime.secondsTens;
						Time.secondsUnits = BackgroundTime.secondsUnits;

					}
					else if(Time.cursorPosition == 6)
					{
						Time.cursorPosition = 7;
						Time.hoursTens = BackgroundTime.hoursTens;
						Time.hoursUnits = BackgroundTime.hoursUnits;


					}
					else if(Time.cursorPosition == 9)
					{
						Time.cursorPosition = 10;
						Time.minutesTens = BackgroundTime.minutesUnits;
						Time.minutesUnits = BackgroundTime.minutesUnits;
					}

					BackgroundTime = Time;
				}
				break;

			case DATE_SWITCH:
				displayStatus ^= 1;
				if(displayStatus == DISPLAY_DATE)
					enableEdit = DISABLED;

				break;
			}
		}
	}



	static u16 secondsCounter = 0 ;
	if(secondsCounter == ONE_SECOND)
	{
		secondsCounter = 0;
		Time.secondsUnits++;
		if(Time.secondsUnits > '9')
		{
			Time.secondsUnits = '0';
			Time.secondsTens++;
			if(Time.secondsTens > '5')
			{
				Time.secondsTens = '0';
				Time.minutesUnits++;
				if(Time.minutesUnits > '9')
				{
					Time.minutesUnits = '0';
					Time.minutesTens++;
					if(Time.minutesTens > '5')
					{
						Time.minutesTens = '0';
						Time.hoursUnits++;
						if((Time.hoursUnits > '3') && (Time.hoursTens == '2'))
						{
							Time.hoursUnits = '0';
							Time.hoursTens = '0';
						}
						else if ( Time.hoursUnits> '9' && (Time.hoursTens < '2'))
						{
							Time.hoursUnits = '0';
							Time.hoursTens++;
						}
					}
				}
			}
		}

	}
	else
		secondsCounter++;

	/*
	 * 1- if edit mode is disabled
	 * 			time->printTime
	 * */

	if (enableEdit == DISABLED)
	{
		PrintTime = Time;

	}
	/*
	 *
	 *  2- else
	 * 				kol marra ha7rak el cursor ha7ot el time f background LAO maghyrtsh 7aga
	 * 				LAO GHAYRT 7AGA			w ba3dain 7arakt el cursour el 7etta elli 3andaha el tagheer tet7at f Time w el ba2y yt7at f background men time
	 * 				print	kol marra ha7ot f print elli f time ma3ada el 7etta elli 3andaha el cursor hakhodha men background
	 *
	 * */
	else
	{
		u8 * printTime  = (u8*)&PrintTime;
		u8 * time       = (u8*)&Time;
		u8 * background = (u8*)&BackgroundTime;
		for (u8 i = 0 ; i < 8 ; i ++)
		{
			if (!(i == (BackgroundTime.cursorPosition-4)))
			{
				printTime[i] = time[i];
			}
			else
			{
				if(BackgroundTime.cursorPosition == 4)
				{
					printTime[i] = background[i];
					i++;
					printTime[i] = background[i];
				}
				else if (BackgroundTime.cursorPosition == 5)
				{
					printTime[i] = background[i];
					printTime[i-1] = background[i-1];
				}
				else if(BackgroundTime.cursorPosition == 7)
				{
					printTime[i] = background[i];
					i++;
					printTime[i] = background[i];
				}
				else if (BackgroundTime.cursorPosition == 8)
				{
					printTime[i] = background[i];
					printTime[i-1] = background[i-1];
				}
				else if(BackgroundTime.cursorPosition == 10)
				{
					printTime[i] = background[i];
					i++;
					printTime[i] = background[i];
				}
				else if (BackgroundTime.cursorPosition == 11)
				{
					printTime[i] = background[i];
					printTime[i-1] = background[i-1];
				}


			}
		}
	}

	/*
	 * 3- if(print != previous)
	 * 		previous = print
	 * 		LCD_print(print)
	 * */

	if ( (TempTime.hoursTens != PrintTime.hoursTens)  && (TempTime.hoursUnits != PrintTime.hoursUnits)  && (TempTime.minutesUnits != PrintTime.minutesUnits)  && (TempTime.minutesTens != PrintTime.minutesTens)  &&(TempTime.secondsUnits != PrintTime.secondsUnits)  &&(TempTime.secondsTens != PrintTime.secondsTens)  && (HLCD_flag != HLCD_FLAG_RAISED))
	{
		TempTime = PrintTime;
		HLCD_flag = HLCD_FLAG_RAISED;
		HLCD_algorithm();
	}
	/*
	 *
	 * -> time
	 * -> TempTime
	 * -> background
	 * -> printTime
	 *
	 * 1- if edit mode is disabled
	 * 			time->printTime
	 * 2- else
	 * 				kol marra ha7rak el cursor ha7ot el time f background LAO maghyrtsh 7aga
	 * 				LAO GHAYRT 7AGA			w ba3dain 7arakt el cursour el 7etta elli 3andaha el tagheer tet7at f Time w el ba2y yt7at f background men time
	 * 				print	kol marra ha7ot f print elli f time ma3ada el 7etta elli 3andaha el cursor hakhodha men background
	 * 	3- if(print != previous)
	 * 		previous = print
	 * 		LCD_print(print)
	 *
	 * */
	UART_errRecieve(RxData, NUM_OF_SWITCHES);

}


void Rx_voidCallBackFunction(void)
{
	RxFlag = 1;
}


static void HLCD_algorithm(void)
{
	static u8 HLCD_u8algorithm = HLCD_CLEAR_STATE;
	switch(HLCD_u8algorithm)
	{
	case HLCD_CLEAR_STATE:
		HLCD_ClearScreen_Runnable();
		HLCD_u8algorithm = HLCD_GOTO_STATE;
	break;

	case HLCD_GOTO_STATE:
		HLCD_GoToXYPos_Runnable(4,0);
		HLCD_u8algorithm = HLCD_DISPLAY_STATE;
	break;

	case HLCD_DISPLAY_STATE:
		HLCD_WriteString_Runnable(PrintTime,8);
		HLCD_u8algorithm = HLCD_CLEAR_STATE;
		HLCD_flag = HLCD_FLAG_LOWERED;
	break;
	}
}





