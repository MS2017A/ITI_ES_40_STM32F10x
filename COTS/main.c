#include "STD_TYPES.h"
#include "DELAY.h"

/* MCAL */
#include "RCC.h"
#include "GPIO_interface.h"
#include"NVIC_interface.h"
#include"uart_interface.h"
#include "STK_interface.h"


/* HAL */
#include "HRCC_interface.h"
#include "HLED_interface.h"
#include "HLCD_interface.h"
#include "HSWITCH_interface.h"

/* Service */
#include "Scheduler_interface.h"

/* APP */
#include "Comm.h"
#include "Display.h"

static GPIO_PinConfig_t UART_pinConofigTx={
		.PIN=UART_Tx_PIN,
		.MODE=MODE_OUTPUT_10_AF_PUSHPULL,
		.PORT=UART_PORT

};

static GPIO_PinConfig_t UART_pinConofigRx={
		.PIN=UART_Rx_PIN,
		.MODE=MODE_INPUT_PULLUP,
		.PORT=UART_PORT

};


uint32_t G_TransCounter=1;
uint32_t G_RecCounter;

uint8_t G_TransBuffer[4]={0};
uint8_t G_RecBuffer[4]={0};

uint8_t TFlag = 0;
uint8_t RFlag = 0;




uint8_t SW_ret;
ErrorStatus_t SW_errFlag = ERROR_OK;

uint8_t flag = 0;
int main(void)
{

	HRCC_EnablePeripheral(HRCC_PERIPHERAL_APB2ENR_IOPAEN);
	HRCC_EnablePeripheral(HRCC_PERIPHERAL_APB2ENR_IOPBEN);
	HRCC_EnablePeripheral(HRCC_PERIPHERAL_APB2ENR_IOPCEN);
	HRCC_EnablePeripheral(HRCC_PERIPHERAL_APB2ENR_USART1EN);
	GPIO_InitStr(&UART_pinConofigTx);
	GPIO_InitStr(&UART_pinConofigRx);


	HLCD_SetNotifyEndofJob_CBF_Runnable(&HLCD_Algorithm);
	SW_errFlag = HSWITCH_Init();
	NVIC_errEnableInterrupt(NVIC_USART1);



	UART_errSetCallBackTrans(TxNotification);
	UART_errSetCallBackRecieve(RxNotification);

	UART_voidInit();

	/* Receive from UART in receive Buffer */
	UART_errRecieve(G_RecBuffer,4);

	Scheduler_Init();
	Scheduler_Start();

	return 0;
}




