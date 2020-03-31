#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H



/*================================================== General #Defines ==================================================*/

#define UART_NOREMAP			0
#define UART_REMAP				1

#define	UART_BAUDRATE_8MHZ								0x341
#define	UART_BAUDRATE_36MHZ								0xEA6
#define	UART_BAUDRATE_72MHZ								0x1D4C



#define	UART_ENABLED_CR1								0x00002000
/*TODO: Anis has added that next line*/
#define UART_DISABLED_CR1								0xFFFFDFFF

/*TODO: Set the clear value*/
#define	UART_8_BITS_CR1									0x00000000
#define	UART_9_BITS_CR1									0X00001000

#define	UART_PARITY_ENABLED								0x00000400
#define	UART_PARITY_DISABLED							0x00000000

#define	UART_PARITY_EVEN								0x00000000
#define	UART_PARITY_ODD									0x00000200

/*TODO: This is not Interrupt enable but Parity Error interrupt enable*/
#define	UART_INTERRUPT_ENABLE							0X00000100
/*TODO: Anis has added that next line*/
#define UART_INTERRUPT_DISABLE							0xFFFFFDFF

/*TODO: i think changing the name to TXEMPTY_INTERRUPT_ENABLE*/
#define	UART_TRANSMIT_INTERRUPT_ENABLE					0X00000080
/*TODO: i think we can add UART_TRANSMIT_INTERRUPT_ENABLE 0x7F*/


/*TODO: i think changing the name to TXEMPTY_INTERRUPT_ENABLE*/
#define	UART_RECIEVE_INTERRUPT_ENABLE					0X00000020
/*TODO: i think we can add UART_RECIEVE_INTERRUPT_DISABLE 0xDF*/

/*TODO: i think changing the name to UART_TRANSMITTER_ENABLE*/
#define	UART_TRANSMIT_ENABLE							0X8
#define	UART_RECIEVE_ENABLE								0X4
/*TODO: Add clear for UART_1_STOP_BIT*/
#define	UART_1_STOP_BIT									0x00000000
#define	UART_0.5_STOP_BIT								0x00001000
#define	UART_2_STOP_BIT									0x00002000
#define	UART_1.5_STOP_BIT								0x00003000



/*******************************************************************
	use any of these  macros to know which error is currently present.
	Please don't change any of these
*********************************************************************/
#define UART_STATUS_PE		(u8)1
#define UART_STATUS_FE		(u8)2
#define UART_STATUS_NE		(u8)4
#define UART_STATUS_ORE		(u8)8
#define UART_STATUS_IDLE	(u8)0x10
#define UART_STATUS_RXNE	(u8)0x20
#define UART_STATUS_TC		(u8)0x40
#define UART_STATUS_TXE		(u8)0x80
#define UART_STATUS_LBD		(u16)0x100
#define UART_STATUS_CTS		(u16)0x200





/*=============================================== General Definitions ==============================================*/

typedef void(*UART_callBack_t)(void);

typedef 	u32	UART_STATUS;






/*=================================================== Prototypes ===================================================*/
void UART_voidInit(void);

u8 UART_errTransmit(u8* Copy_pu8Data, u16 Copy_u16Size);

u8 UART_errRecieve(u8* Copy_pu8Data, u16 Copy_u16ize);

u8 UART_errSetCallBackTrans(UART_callBack_t UART_callBack);
u8 UART_errSetCallBackRecieve(UART_callBack_t UART_callBack);



UART_STATUS UART_u16GetStatus(void);

#endif
