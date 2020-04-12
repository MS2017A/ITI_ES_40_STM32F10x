
#ifndef LED_CNFG_H_
#define LED_CNFG_H_
#include "GPIO_interface.h"



#define NUM_LEDS   3



/*Options:                                      */
/*        P_NUMx                                */
/*        x->  (0-15)                           */


#define LED1_Pin	P_NUM13
#define LED2_Pin 	P_NUM13
#define LED3_Pin	P_NUM2


/*Options:                                      */
/*        PORTA                                 */
/*        PORTB                                 */
/*        PORTC                                 */



#define LED1_Port		PORTC
#define LED2_Port		PORTA
#define LED3_Port		PORTA


/*Options:                                      */
/*        HIGH                                  */
/*        LOW                                   */


#define SET				HIGH
#define RESET			LOW


/*Options:                                      */
/*        MODE_OUTPUT_2_GP_PUSHPULL             */
/*        MODE_OUTPUT_10_GP_PUSHPULL             */
/*        MODE_OUTPUT_50_GP_PUSHPULL             */

#define LED1_MODE		MODE_OUTPUT_2_GP_PUSHPULL
#define LED2_MODE 	MODE_OUTPUT_10_GP_PUSHPULL
#define LED3_MODE		MODE_OUTPUT_50_GP_PUSHPULL

#define LED_MODE	  MODE_OUTPUT_2_GP_PUSHPULL

STD_ERROR LED_GetStruct(u32 LED_Name,LED_t *CopyLed);
STD_ERROR LED_GetPIN(void* PORT,u32 *PIN);


#endif /* LED_CNFG_H_ */
