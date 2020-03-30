/*
 * HLCD_prog.c
 *
 *  Created on: Mar 18, 2020
 *      Author: madel
 */


#include "STD_TYPES.h"
#include "DELAY.h"

#include "GPIO_interface.h"

#include "HLCD_interface.h"
#include "HLCD_config.h"
#include "Scheduler_interface.h"

/************************************* Static Prototypes ******************************************/
static void HLCD_WriteCommand(uint8_t Command);
static void HLCD_WriteData(uint8_t Data);
static void Enable_Pulse(void);
static void Init_4Bits_Mode(void);



/*********************************** General Used #defines *****************************************/
#if LCD_MODE == EIGHT_BITS_MODE

#define RS_PIN	(0)
#define RW_PIN	(1)
#define EN_PIN	(2)

#define D0_PIN	(3)
#define D1_PIN	(4)
#define D2_PIN	(5)
#define D3_PIN	(6)
#define D4_PIN	(7)
#define D5_PIN	(8)
#define D6_PIN	(9)
#define D7_PIN	(10)

#elif	LCD_MODE == FOUR_BITS_MODE

#define RS_PIN	(0)
#define RW_PIN	(1)
#define EN_PIN	(2)

#define D4_PIN	(3)
#define D5_PIN	(4)
#define D6_PIN	(5)
#define D7_PIN	(6)

#endif



#define SECOND_ROW_ADD_OFFSET		(0x40)
#define SET_DDRAM_ADDRESS_CMD		(0x80)

#define MAXIMUM_X_AXIS_CHARACTER	(16)
#define MAXIMUM_Y_AXIS_CHARACTER	(2)

/****************************************** Static Prototypes of Scheduler ***********************************************/

#if LCD_MODE == EIGHT_BITS_MODE

static void HLCD_Init_EightBits_Runnable(void);
static void HLCD_write_EightBitsData_Runnable(void);
static void HLCD_ExecuteCommand_EightBitsData_Runnable(uint8_t Command);

#elif LCD_MODE == FOUR_BITS_MODE

static void HLCD_Init_FourBits_Runnable(void);
static void HLCD_write_FourBitsData_Runnable(void);
static void HLCD_ExecuteCommand_FourBitsData_Runnable(uint8_t Command);

#endif

/**************************************** General Definitions for the Task ***********************************************/

/* extern task */
extern Task_t HLCD_Scheduler_Task;

/* The different states of the LCD operations */
typedef enum{

	HLCD_Init_State,
    HLCD_Ideal_State,
	HLCD_Freeze_State,
    HLCD_writeString_State,
    HLCD_ClearScreen_State,
    HLCD_CursorMove_State,

}HLCD_RequestState_t;

/* Date buffer */
typedef struct{

  uint8_t  Data[32];
  uint8_t  Size;

}DateBuffer_t;



/* Creating new instance of the Data Buffer*/
static DateBuffer_t HLCD_DateBuffer ;


/* Creating new instance of the Position of Cursor */
static uint8_t Global_DDRamAdd = 0;



/* Global variable that will hold the current state of the HLCD "The HLCD should initialized with HLCD_Init" */
static HLCD_RequestState_t	 CurrentWorkingState = HLCD_Init_State;


/* Global Counter that will stop the system for specific amount of time */
static uint32_t Freeze_Time = 0;

/* Notify the Application Type */
typedef 	void(*HLCD_NotifyEndofJob_t)(void);
static HLCD_NotifyEndofJob_t	HLCD_NotifyEndofJob;



#define Clear_SCREEN_COMMAND	(0x01)
/************************************** Implementations OF APIs *********************************************/

void HLCD_Init(void)
{

	uint8_t Command = 0x00;
	/* Wait more than 30 mill-seconds for initialization the LCD */
	Delay_ms(40);


	#if LCD_MODE == EIGHT_BITS_MODE

		for(uint8_t index=0 ; index<11 ; index++)
		{
			GPIO_InitStr( &(HLCD_PinsConfig[index].GPIO_PinConfig) );
		}

		/* Notify the LCD that we are going to operate on 8 bits mode & Set number of lines enabled & The Font */
		Command = ( 0x30 | LCD_NUM_LINE | LCD_FONT );
		HLCD_WriteCommand(Command);

		/* Enable the display of LCD & configure Blinking and cursor of the LCD */
		Command = 0x00;
		Command = ( 0x0C | LCD_CURSOR_MODE | LCD_BLINK_MODE );
		HLCD_WriteCommand(Command);

		/* Clear the display */
		Command = 0x01;
		HLCD_WriteCommand(Command);

	#elif	LCD_MODE == FOUR_BITS_MODE

		for(uint8_t index=0 ; index<7 ; index++)
		{
			GPIO_InitStr( &(HLCD_PinsConfig[index].GPIO_PinConfig) );
		}

		/* Notify the LCD that we are going to operate on 4 bits mode */
		Init_4Bits_Mode();


		/* Set number of lines enabled & The Font */
		Command = 0x20;
		Command = (LCD_NUM_LINE | LCD_FONT);
		HLCD_WriteCommand(Command);

		/* Enable the display of LCD & configure Blinking and cursor of the LCD */
		Command = 0x00;
		Command = (0x0C | LCD_CURSOR_MODE | LCD_BLINK_MODE) ;
		HLCD_WriteCommand(Command);

		/* Clear the display */
		Command = 0x01 ;
		HLCD_WriteCommand(Command);

	#endif
}


void HLCD_WriteString(const char* Needed_String)
{
	/* Don't stop the while until the end of the string */
	while( *Needed_String != '\0')
	{
		/* Write the Character on the LCD */
		HLCD_WriteData(*Needed_String);

		/* Increment the pointer that we get the next character in the string */
		Needed_String++;
	}
}


void HLCD_GoToXYPos(uint8_t XPos , uint8_t YPos)
{
	u8 Local_u8DDRamAdd=0;
	if(YPos == 0)
	{
		Local_u8DDRamAdd = XPos;
	}
	else if(YPos == 1)
	{
		Local_u8DDRamAdd = XPos + SECOND_ROW_ADD_OFFSET;
	}
	else
	{
		/*Do Nothing*/
	}
	HLCD_WriteCommand(Local_u8DDRamAdd | SET_DDRAM_ADDRESS_CMD);
}

/************************************** Implementations OF Scheduler APIs *********************************************/

HLCD_ErrorStatus_t HLCD_WriteString_Runnable(const uint8_t* Needed_String , uint8_t Size)
{
	HLCD_ErrorStatus_t LocalError = HLCD_ERROR_OK;
	uint8_t Data_index = 0;

	if(CurrentWorkingState == HLCD_Ideal_State)
	{
		if(Needed_String)
		{

			CurrentWorkingState = HLCD_writeString_State;

			for( Data_index=0 ; Data_index<Size ; Data_index++)
			{
				HLCD_DateBuffer.Data[Data_index] = Needed_String[Data_index];
			}
			HLCD_DateBuffer.Size = Size;
		}
		else
		{
			LocalError |= HLCD_ERROR_NULL_POINTER;
		}

	}
	else
	{
		LocalError |= HLCD_ERROR_BUSY;
	}

	return	LocalError;
}


HLCD_ErrorStatus_t HLCD_GoToXYPos_Runnable(uint8_t XPos , uint8_t YPos)
{

	HLCD_ErrorStatus_t LocalError = HLCD_ERROR_OK;
	uint8_t Local_DDRamAdd = 0;

	if(CurrentWorkingState == HLCD_Ideal_State)
	{
		if(XPos < MAXIMUM_X_AXIS_CHARACTER  &&  YPos < MAXIMUM_Y_AXIS_CHARACTER)
		{

			CurrentWorkingState = HLCD_CursorMove_State;

			if(YPos == HLCD_FIRST_ROW)
			{
				Local_DDRamAdd = XPos;
			}
			else if(YPos == HLCD_SECOND_ROW)
			{
				Local_DDRamAdd = XPos + SECOND_ROW_ADD_OFFSET;
			}
			else
			{
				/*Do Nothing*/
			}

			Global_DDRamAdd = Local_DDRamAdd | SET_DDRAM_ADDRESS_CMD;
		}
		else
		{
			LocalError |= HLCD_ERROR_NOK;
		}

	}
	else
	{
		LocalError |= HLCD_ERROR_BUSY;
	}

	return	LocalError;
}


HLCD_ErrorStatus_t HLCD_ClearScreen_Runnable(void)
{
	HLCD_ErrorStatus_t LocalError = HLCD_ERROR_OK;

	if(CurrentWorkingState == HLCD_Ideal_State)
	{

		CurrentWorkingState = HLCD_ClearScreen_State;

	}
	else
	{
		LocalError = HLCD_ERROR_BUSY;
	}

	return	LocalError;
}


HLCD_ErrorStatus_t SetNotifyEndofJob_CBF_Runnable(void(*HLCD_NotifyEndofJob_CBF)(void))
{
	HLCD_ErrorStatus_t LocalError = ERROR_OK;

	if(HLCD_NotifyEndofJob_CBF)
	{
		HLCD_NotifyEndofJob = HLCD_NotifyEndofJob_CBF;
	}
	else
	{
		LocalError |= HLCD_ERROR_NULL_POINTER;
	}

	return LocalError;
}

HLCD_ErrorStatus_t HLCD_FreezeTheSystem(uint32_t Freeze_Time_ms)
{
	HLCD_ErrorStatus_t LocalError = HLCD_ERROR_OK;
	if(CurrentWorkingState == HLCD_Ideal_State)
	{
		CurrentWorkingState = HLCD_Freeze_State;
		Freeze_Time = (Freeze_Time_ms/HLCD_Scheduler_Task.Periodicity_ms)+1;
	}
	else
	{
		LocalError = HLCD_ERROR_BUSY;
	}

	return LocalError;
}

/**************************************** Implementation of Task ***********************************************/

/* Task coming every Periodicity */
void HLCD_Runnable(void)
{
	switch(CurrentWorkingState)
	{
	case HLCD_Init_State:
				{
				#if LCD_MODE == EIGHT_BITS_MODE

					HLCD_Init_EightBits_Runnable();

				#elif LCD_MODE == FOUR_BITS_MODE

					HLCD_Init_FourBits_Runnable();

				#endif
				}
		break;

	 case HLCD_Freeze_State:
				{
					static uint32_t Freeze_Counter = 0;

					if(Freeze_Counter < Freeze_Time)
					{
						Freeze_Counter++;
					}
					else
					{
						Freeze_Counter = 0;
						Freeze_Time = 0;
						CurrentWorkingState = HLCD_Ideal_State;
						HLCD_NotifyEndofJob();
					}
				}
	    break;

	case HLCD_Ideal_State:
				{

				}
    	break;

    case HLCD_writeString_State:
				{
				#if LCD_MODE == EIGHT_BITS_MODE

					HLCD_write_EightBitsData_Runnable();

				#elif LCD_MODE == FOUR_BITS_MODE

					HLCD_write_FourBitsData_Runnable();

				#endif
				}
    	break;

    case HLCD_ClearScreen_State:
    			{
				#if LCD_MODE == EIGHT_BITS_MODE

    				HLCD_ExecuteCommand_EightBitsData_Runnable(Clear_SCREEN_COMMAND);

				#elif LCD_MODE == FOUR_BITS_MODE

    				HLCD_ExecuteCommand_FourBitsData_Runnable(Clear_SCREEN_COMMAND);

				#endif
				}
    	break;

    case HLCD_CursorMove_State:
    			{

				#if LCD_MODE == EIGHT_BITS_MODE

    				HLCD_ExecuteCommand_EightBitsData_Runnable(Global_DDRamAdd);

				#elif LCD_MODE == FOUR_BITS_MODE

					HLCD_ExecuteCommand_FourBitsData_Runnable(Global_DDRamAdd);

				#endif
				}
    	break;

	}
}


Task_t HLCD_Scheduler_Task={

		.Runnable 	    = &HLCD_Runnable ,
		.Periodicity_ms = 2 			 ,
		.State			= TASK_RUNNING_STATE

};


/*============================= Implementation of Static Prototypes of Scheduler ===================================*/


#if LCD_MODE == EIGHT_BITS_MODE

static void HLCD_Init_EightBits_Runnable(void)
{

	static uint8_t Counter          = 0;
	static uint8_t Init_Counter     = 1;
	static uint8_t Command          = 0;
	static uint8_t EnablePin_Status = 0;



	uint8_t Delay_Count             = (uint8_t) (40 / (HLCD_Scheduler_Task.Periodicity_ms) );

	if(Counter <= Delay_Count)
	{
		if(Counter == 0)
		{
			for(uint8_t index=0 ; index<LCD_MODE ; index++)
				{
					GPIO_InitStr( &(HLCD_PinsConfig[index].GPIO_PinConfig) );
				}
			Counter++;
		}
		else
		{
			Counter++;
		}

	}
	else
	{
		switch(Init_Counter)
		{
			case 1:
					Command = ( 0x30 | LCD_NUM_LINE | LCD_FONT );
				break;

			case 2:
					Command = ( 0x0C | LCD_CURSOR_MODE | LCD_BLINK_MODE );
				break;

			case 3:
					Command = 0x01;
				break;

		}

		if(EnablePin_Status == HIGH)
		{
			/* Set the Enable pin LOW */
			GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , LOW );
			EnablePin_Status = LOW;
			Init_Counter++;

			if(Init_Counter == 4)
			{
				/* Return the system to Ideal State */
				CurrentWorkingState = HLCD_Ideal_State;
				HLCD_NotifyEndofJob();
			}
		}
		else
		{
				/* Write on RS & RW Pin */
				GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , LOW );
				GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW  );

				/* Write the Data */
				GPIO_SetPinValue( &(HLCD_PinsConfig[D0_PIN].GPIO_PinConfig) , ((Command & 0x01)     ));
				GPIO_SetPinValue( &(HLCD_PinsConfig[D1_PIN].GPIO_PinConfig) , ((Command & 0x02) >> 1));
				GPIO_SetPinValue( &(HLCD_PinsConfig[D2_PIN].GPIO_PinConfig) , ((Command & 0x04) >> 2));
				GPIO_SetPinValue( &(HLCD_PinsConfig[D3_PIN].GPIO_PinConfig) , ((Command & 0x08) >> 3));
				GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Command & 0x10) >> 4));
				GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Command & 0x20) >> 5));
				GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Command & 0x40) >> 6));
				GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Command & 0x80) >> 7));

				/* Set the Enable pin High */
				GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , HIGH );
				EnablePin_Status = HIGH;

		}
	}
}

static void HLCD_write_EightBitsData_Runnable(void)
{

	static uint8_t Data_Counter     = 0;
	static uint8_t EnablePin_Status = 0;


	if(EnablePin_Status == 1)
	{
		/* Set the Enable pin LOW */
		GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , LOW );
		EnablePin_Status = LOW;

		/* increase the Data counter */
		Data_Counter++;

		/*Check the number of the Data*/
		if( Data_Counter == HLCD_DateBuffer.Size)
			{

				HLCD_DateBuffer.Size = 0;
				Data_Counter=0;
				CurrentWorkingState = HLCD_Ideal_State;
				HLCD_NotifyEndofJob();
			}

	}
	else
	{
			/* Write on RS & RW Pin */
			GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , HIGH );
			GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW  );

			/* Write the Data */
			GPIO_SetPinValue( &(HLCD_PinsConfig[D0_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x01)     ));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D1_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x02) >> 1));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D2_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x04) >> 2));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D3_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x08) >> 3));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x10) >> 4));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x20) >> 5));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x40) >> 6));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x80) >> 7));

			/* Set the Enable pin High */
			GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , HIGH );
			EnablePin_Status = HIGH;

	}
}

static void HLCD_ExecuteCommand_EightBitsData_Runnable(uint8_t Command)
{

	static uint8_t EnablePin_Status = 0;


	if(EnablePin_Status == 1)
	{
		/* Set the Enable pin LOW */
		GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , LOW );
		EnablePin_Status = LOW;

		/* Return the system to Ideal State */
		CurrentWorkingState = HLCD_Ideal_State;
		HLCD_NotifyEndofJob();


	}
	else
	{
			/* Write on RS & RW Pin */
			GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , LOW );
			GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW  );

			/* Write the Data */
			GPIO_SetPinValue( &(HLCD_PinsConfig[D0_PIN].GPIO_PinConfig) , ((Command & 0x01)     ));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D1_PIN].GPIO_PinConfig) , ((Command & 0x02) >> 1));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D2_PIN].GPIO_PinConfig) , ((Command & 0x04) >> 2));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D3_PIN].GPIO_PinConfig) , ((Command & 0x08) >> 3));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Command & 0x10) >> 4));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Command & 0x20) >> 5));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Command & 0x40) >> 6));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Command & 0x80) >> 7));

			/* Set the Enable pin High */
			GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , HIGH );
			EnablePin_Status = HIGH;

	}
}


#elif LCD_MODE == FOUR_BITS_MODE


static void HLCD_Init_FourBits_Runnable(void)
{
		static uint8_t Counter               	 = 0;
		static uint8_t Init_Counter          	 = 1;
		static uint8_t Command               	 = 0;
		static uint8_t Extra_Half_Command_Flag	 = 0;
		static uint8_t EnablePin_4bit_Status 	 = 0;

		uint8_t Delay_Count             = (uint8_t) (40 / (HLCD_Scheduler_Task.Periodicity_ms));

		if(Counter <= Delay_Count)
		{
			if(Counter == 0)
			{
				for(uint8_t index=0 ; index<LCD_MODE ; index++)
					{
						GPIO_InitStr( &(HLCD_PinsConfig[index].GPIO_PinConfig) );
					}
				Counter++;
			}
			else
			{
				Counter++;
			}

		}
		else
		{
			switch(Init_Counter)
			{
				case 1:
						Command = ( 0x20 | LCD_NUM_LINE | LCD_FONT );
					break;

				case 2:
						Command = ( 0x0C | LCD_CURSOR_MODE | LCD_BLINK_MODE );
					break;

				case 3:
						Command = 0x01;
					break;

			}

			if(EnablePin_4bit_Status == 3)
				{
					/* Set the Enable pin LOW */
					GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , LOW );
					EnablePin_4bit_Status = 0;
					Init_Counter++;

					/* Return the system to Ideal State */
					Extra_Half_Command_Flag = 0;
					CurrentWorkingState = HLCD_Ideal_State;
					HLCD_NotifyEndofJob();


				}
				else if(EnablePin_4bit_Status == 2)
				{
					/* Write on RS & RW Pin */
					GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , LOW );
					GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW );

					/* Write the first 4 bits Data */
					GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Command & 0x01)     ));
					GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Command & 0x02) >> 1));
					GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Command & 0x04) >> 2));
					GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Command & 0x08) >> 3));

					/* Set the Enable pin High */
					GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , HIGH );
					EnablePin_4bit_Status++;
				}
				else if(EnablePin_4bit_Status == 1)
				{
					/* Set the Enable pin LOW */
					GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , LOW );
					EnablePin_4bit_Status++;

					if(Extra_Half_Command_Flag == 0)
					{
						EnablePin_4bit_Status = 0;
						Extra_Half_Command_Flag=1;
					}
				}
				else
				{
						/* Write on RS & RW Pin */
						GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , LOW );
						GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW  );

						/* Write the first 4 bits Data */
						GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Command & 0x10) >> 4));
						GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Command & 0x20) >> 5));
						GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Command & 0x40) >> 6));
						GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Command & 0x80) >> 7));

						/* Set the Enable pin High */
						GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , HIGH );
						EnablePin_4bit_Status++;

				}



		}
}

static void HLCD_write_FourBitsData_Runnable(void)
{
	static uint8_t Data_Counter     = 0;
	static uint8_t EnablePin_4bit_Status = 0;


	if(EnablePin_4bit_Status == 3)
	{
		/* Set the Enable pin LOW */
		GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , LOW );
		EnablePin_4bit_Status = 0;

		/* increase the Data counter */
		Data_Counter++;

		/*Check the number of the Data*/
		if( Data_Counter == HLCD_DateBuffer.Size)
			{
				HLCD_DateBuffer.Size = 0;
				Data_Counter=0;
				CurrentWorkingState = HLCD_Ideal_State;
				HLCD_NotifyEndofJob();
			}

	}
	else if(EnablePin_4bit_Status == 2)
	{
		/* Write on RS & RW Pin */
		GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , HIGH );
		GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW  );

		/* Write the first 4 bits Data */
		GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x01)     ));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x02) >> 1));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x04) >> 2));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x08) >> 3));

		/* Set the Enable pin High */
		GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , HIGH );
		EnablePin_4bit_Status++;
	}
	else if(EnablePin_4bit_Status == 1)
	{
		/* Set the Enable pin LOW */
		GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , LOW );
		EnablePin_4bit_Status++;
	}
	else
	{
			/* Write on RS & RW Pin */
			GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , HIGH );
			GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW  );

			/* Write the first 4 bits Data */
			GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x10) >> 4));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x20) >> 5));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x40) >> 6));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((HLCD_DateBuffer.Data[Data_Counter] & 0x80) >> 7));

			/* Set the Enable pin High */
			GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , HIGH );
			EnablePin_4bit_Status++;

	}
}

static void HLCD_ExecuteCommand_FourBitsData_Runnable(uint8_t Command)
{
	static uint8_t EnablePin_4bit_Status = 0;


	if(EnablePin_4bit_Status == 3)
	{
		/* Set the Enable pin LOW */
		GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , LOW );
		EnablePin_4bit_Status = 0;

		/* Return the system to Ideal State */
		CurrentWorkingState = HLCD_Ideal_State;
		HLCD_NotifyEndofJob();


	}
	else if(EnablePin_4bit_Status == 2)
	{
		/* Write on RS & RW Pin */
		GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , LOW );
		GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW );

		/* Write the first 4 bits Data */
		GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Command & 0x01)     ));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Command & 0x02) >> 1));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Command & 0x04) >> 2));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Command & 0x08) >> 3));

		/* Set the Enable pin High */
		GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , HIGH );
		EnablePin_4bit_Status++;
	}
	else if(EnablePin_4bit_Status == 1)
	{
		/* Set the Enable pin LOW */
		GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , LOW );
		EnablePin_4bit_Status++;
	}
	else
	{
			/* Write on RS & RW Pin */
			GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , LOW );
			GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW  );

			/* Write the first 4 bits Data */
			GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Command & 0x10) >> 4));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Command & 0x20) >> 5));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Command & 0x40) >> 6));
			GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Command & 0x80) >> 7));

			/* Set the Enable pin High */
			GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , HIGH );
			EnablePin_4bit_Status++;

	}
}

#endif


/**************************************** Implementation of Static Functions ***********************************************/



static void HLCD_WriteCommand(uint8_t Command)
{

	GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig ) , LOW );
	GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig ) , LOW );

	#if LCD_MODE == EIGHT_BITS_MODE

		GPIO_SetPinValue( &(HLCD_PinsConfig[D0_PIN].GPIO_PinConfig ) , ((Command & 0x01)    ));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D1_PIN].GPIO_PinConfig ) , ((Command & 0x02) >>1));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D2_PIN].GPIO_PinConfig ) , ((Command & 0x04) >>2));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D3_PIN].GPIO_PinConfig ) , ((Command & 0x08) >>3));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig ) , ((Command & 0x10) >>4));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig ) , ((Command & 0x20) >>5));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig ) , ((Command & 0x40) >>6));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig ) , ((Command & 0x80) >>7));

	#elif	LCD_MODE == FOUR_BITS_MODE

		GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Command & 0x10) >> 4));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Command & 0x20) >> 5));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Command & 0x40) >> 6));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Command & 0x80) >> 7));

		Enable_Pulse();

		GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig ) , LOW );
		GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig ) , LOW );


		GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Command & 0x01)     ));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Command & 0x02) >> 1));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Command & 0x04) >> 2));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Command & 0x08) >> 3));

	#endif

	Enable_Pulse();
}

 static void HLCD_WriteData(uint8_t Data)
{

	GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , HIGH );
	GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW  );

	#if LCD_MODE == EIGHT_BITS_MODE

		GPIO_SetPinValue( &(HLCD_PinsConfig[D0_PIN].GPIO_PinConfig) , ((Data & 0x01)     ));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D1_PIN].GPIO_PinConfig) , ((Data & 0x02) >> 1));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D2_PIN].GPIO_PinConfig) , ((Data & 0x04) >> 2));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D3_PIN].GPIO_PinConfig) , ((Data & 0x08) >> 3));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Data & 0x10) >> 4));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Data & 0x20) >> 5));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Data & 0x40) >> 6));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Data & 0x80) >> 7));

	#elif	LCD_MODE == FOUR_BITS_MODE

		GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Data & 0x10) >> 4));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Data & 0x20) >> 5));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Data & 0x40) >> 6));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Data & 0x80) >> 7));

		Enable_Pulse();

		GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig) , HIGH );
		GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig) , LOW  );


		GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Data & 0x01)     ));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Data & 0x02) >> 1));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Data & 0x04) >> 2));
		GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Data & 0x08) >> 3));

	#endif

	Enable_Pulse();

}

static void Enable_Pulse(void)
{
	GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , HIGH );
	Delay_ms(3);
	GPIO_SetPinValue( &(HLCD_PinsConfig[EN_PIN].GPIO_PinConfig) , LOW  );
	Delay_ms(3);
}

static void Init_4Bits_Mode(void)
{
	uint8_t Command = 0x02;
	GPIO_SetPinValue( &(HLCD_PinsConfig[RS_PIN].GPIO_PinConfig ) , LOW );
	GPIO_SetPinValue( &(HLCD_PinsConfig[RW_PIN].GPIO_PinConfig ) , LOW );

	GPIO_SetPinValue( &(HLCD_PinsConfig[D4_PIN].GPIO_PinConfig) , ((Command & 0x01)     ));
	GPIO_SetPinValue( &(HLCD_PinsConfig[D5_PIN].GPIO_PinConfig) , ((Command & 0x02) >> 1));
	GPIO_SetPinValue( &(HLCD_PinsConfig[D6_PIN].GPIO_PinConfig) , ((Command & 0x04) >> 2));
	GPIO_SetPinValue( &(HLCD_PinsConfig[D7_PIN].GPIO_PinConfig) , ((Command & 0x08) >> 3));

	Enable_Pulse();
}
