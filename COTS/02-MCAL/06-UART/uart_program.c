#include "std_types.h"
#include "bit_math.h"
#include "GPIO_int.h"
#include"NVIC_interface.h"
#include "uart_interface.h"
#include "uart_config.h"

#if UART_AFIO == UART_NOREMAP

#define UART_PORT		    PORTA
#define	UART_Tx_PIN			PIN_9
#define	UART_Rx_PIN			PIN_10

#elif UART_AFIO == UART_REMAP

#define UART_PORT			PORTB
#define	UART_Tx_PIN			PIN_6
#define	UART_Rx_PIN			PIN_7

#endif





typedef struct
{
	u32 SR;
	u32 DR;
	u32 BRR;
	u32 CR1;
	u32 CR2;
	u32 CR3;
	u32 GTPR;
}UART_t;

#define UART_BASEADDRESS		0x40013800

static volatile UART_t* UART = ((volatile UART_t*)UART_BASEADDRESS);



typedef struct 
{
	u8* data;
	u16 size;
	u16 position;
	u8 state;
}UART_data_t;

UART_data_t UART_TxBuffer;
UART_data_t UART_RxBuffer;

UART_callBack_t UART_EndOfJobNotification_Tx;
UART_callBack_t UART_EndOfJobNotification_Rx;




static GPIO_t UART_pinConofigTx={
		.pin=UART_Tx_PIN,
		.speed=SPEED_10_MHZ,			//TODO  as configuration
		.mode=MODE_OUTPUT_AF_PUSH_PULL,
		.port=UART_PORT

};

static GPIO_t UART_pinConofigRx={
		.pin=UART_Rx_PIN,
		.speed=0,
		.mode=MODE_INPUT_FLOATING,
		.port=UART_PORT

};



void UART_voidInit(void)
{

	GPIO_voidinitPins(&UART_pinConofigTx);
	GPIO_voidinitPins(&UART_pinConofigRx);



	UART->BRR = UART_CLOCK;			//baudrate


	UART->CR1 = UART_CR1;			//Enable Interrupt -  - parity

	UART->CR2 = UART_CR2;			//stop bit


}

/*void UART_voidCnfg(UARTCnfg_t configuration)
{

	//Baudrate 
	//STB
	//parity
	//Enable Interrupt 

}*/

void USART1_IRQHandler(void)
{

	if(UART->SR & UART_STATUS_TXE)
	{
		if(UART_TxBuffer.position < UART_TxBuffer.size)
		{
			UART->DR = UART_TxBuffer.data[UART_TxBuffer.position];
			UART_TxBuffer.position ++;

		}
		else
		{
			UART_TxBuffer.state = OK;

			UART_TxBuffer.data = NULL;
			UART_TxBuffer.size = 0;
			UART_TxBuffer.position = 0;
			UART->CR1&=~UART_TRANSMIT_INTERRUPT_ENABLE;

			if(UART_EndOfJobNotification_Tx)
				UART_EndOfJobNotification_Tx();
		}

	}
	if(UART->SR & UART_STATUS_RXNE)
	{
		if(UART_RxBuffer.state == BUSY)
		{
			UART_RxBuffer.position ++;
			UART_RxBuffer.data[UART_RxBuffer.position] = UART->DR;
		}
		if(UART_RxBuffer.position == UART_RxBuffer.size)
		{
			UART_RxBuffer.data = NULL;
			UART_RxBuffer.size = 0;
			UART_RxBuffer.position = 0;
			UART_RxBuffer.state = OK;
			if(UART_EndOfJobNotification_Rx)
				UART_EndOfJobNotification_Rx();
		}
	}

}

u8 UART_errTransmit(u8* Copy_pu8Data, u16 Copy_u16Size)
{
	u8 Error_s = NULLPOINTER;
	if(Copy_pu8Data)
	{
		Error_s = BUSY;
		if(UART_TxBuffer.state == OK)
		{
			Error_s = OK;
			UART_TxBuffer.state = BUSY;
			UART_TxBuffer.data = Copy_pu8Data;
			UART_TxBuffer.size = Copy_u16Size;
			UART->DR = UART_TxBuffer.data[0];
			UART_TxBuffer.position++;
			UART->CR1|=UART_TRANSMIT_INTERRUPT_ENABLE;

		}
	}
	return Error_s;
}

u8 UART_errRecieve(u8* Copy_pu8Data, u16 Copy_u16Size)
{
	u8 Error_s = NULLPOINTER;
	if(Copy_pu8Data)
	{
		Error_s = BUSY;
		if(UART_RxBuffer.state == OK)
		{
			Error_s = OK;
			UART_RxBuffer.state = BUSY;
			UART_RxBuffer.data = Copy_pu8Data;
			UART_RxBuffer.size = Copy_u16Size;
		}
	}
	return Error_s;
}

u8 UART_errSetCallBackTrans(UART_callBack_t UART_callBack)
{
	u8 Error_st = NULLPOINTER;
	if(UART_callBack)
	{
		Error_st = OK;
		UART_EndOfJobNotification_Tx = UART_callBack;
	}
	return Error_st;
}
u8 UART_errSetCallBackRecieve(UART_callBack_t UART_callBack)
{
	u8 Error_st = NULLPOINTER;
	if(UART_callBack)
	{
		Error_st = OK;
		UART_EndOfJobNotification_Rx = UART_callBack;
	}
	return Error_st;
}


UART_STATUS UART_u16GetStatus(void)
{
	return UART->SR;
}
