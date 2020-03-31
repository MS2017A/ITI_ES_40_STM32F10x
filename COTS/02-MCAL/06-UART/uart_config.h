#ifndef UART_CONFIG_H
#define UART_CONFIG_H

/******************************

OPTIONS:
			1-UART_NOREMAP
			2-UART_REMAP

*******************************/
#define	UART_AFIO		UART_NOREMAP


#define	UART_CLOCK	UART_BAUDRATE_8MHZ


/******************************

OPTIONS:
			
				UART_BAUDRATE_8MHZ							
				UART_BAUDRATE_36MHZ							
				UART_ENABLE_CR1									
				UART_8_BITS_CR1									
				UART_9_BITS_CR1									
				UART_PARITY_ENABLED							
				UART_PARITY_DISABLED						
				UART_PARITY_EVEN								
				UART_PARITY_ODD									
				UART_INTERRUPT_ENABLE						
				UART_TRANSMIT_INTERRUPT_ENABLE	
				UART_RECIEVE_INTERRUPT_ENABLE		
				UART_TRANSMIT_ENABLE						
				UART_RECIEVE_ENABLE							

*******************************/



#define	UART_CR1	(UART_ENABLED_CR1 |UART_9_BITS_CR1 | UART_PARITY_ENABLED | UART_PARITY_ODD |UART_INTERRUPT_ENABLE  | UART_RECIEVE_INTERRUPT_ENABLE | UART_TRANSMIT_ENABLE | UART_RECIEVE_ENABLE)


#define	UART_CR2	UART_1_STOP_BIT





/*TODO: ANIS has added the next Lines */


/***************************************************************
 * Description: This configuration configures the initialized status
 * 				of UART
 *
 * Options:
 * 		1- UART_ENABLED_CR1
 * 		2- UART_DISABLED_CR1
 ***************************************************************/
#define 	UART_INIT_CONFIG			UART_ENABLED_CR1


/***************************************************************
 * Description: This configuration configures the USART BaudRate
 *
 * Options:
 * 		1- UART_BAUDRATE_8MHZ
 * 		2- UART_BAUDRATE_36MHZ
 ***************************************************************/
#define 	UART_BAUDRATE			UART_BAUDRATE_72MHZ


/***************************************************************
 * Description: This configuration configures the number Data Bits
 *
 * Options:
 * 		1- UART_8_BITS_CR1
 * 		2- UART_9_BITS_CR1
 ***************************************************************/
#define 	UART_DATA_BITS_NUM		UART_8_BITS_CR1



/***************************************************************
 * Description: This configuration enables and disables the parity
 * 				bit in UART Frame
 *
 * Options:
 * 		1- UART_PARITY_ENABLED
 * 		2- UART_PARITY_DISABLED
 ***************************************************************/
#define 	UART_PARITY_STATE		UART_PARITY_ENABLED



/***************************************************************
 * Description: This configuration configures the type of parity
 * 				bit in UART Frame
 *
 * Options:
 * 		1- UART_PARITY_EVEN
 * 		2- UART_PARITY_ODD
 ***************************************************************/
#define 	UART_PARITY_STATE		UART_PARITY_EVEN

















#endif
