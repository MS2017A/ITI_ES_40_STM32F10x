#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include"RCC.h"
#include "HRCC_interfac.h"


#define HRCC_CLOCK_TIME_OUT  10000U

/*This Function to turn on the clock and to check if the readybit is set or not */
/*Inputs:                                                                 */
/*          1)RCC_CFGR_SW_HSI                                             */
/*          2)RCC_CFGR_SW_HSE                                             */
/*          3)RCC_CFGR_SW_PLL                                             */
/*Output:                                                                 */
/*          1)OK                                                          */
/*          2)NOK                                                         */


static STD_ERROR HRCC_SetClockON(u32 RCC_CFGR_SW)
{
    /*Switch The clock Status to ON                                         */
  RCC_SetClock_Status(RCC_CFGR_SW,RCC_STATUS_ON);
  u32 Counter=0;
  STD_ERROR LocalError=OK;
  /*Check if the Clock is turnned On or Not                               */
   if(RCC_CFGR_SW==RCC_CFGR_SW_HSI)
   {
    /*Timeout mechanism                                                   */
    while(!RCC_GetClock_Status(RCC_CFGR_SW_HSI) && Counter<=HRCC_CLOCK_TIME_OUT)
    {
      Counter++;
    }
   }
   else if(!RCC_CFGR_SW==RCC_CFGR_SW_HSE)
   {
    /*Timeout mechanism                                                   */
    while(!RCC_GetClock_Status(RCC_CFGR_SW_HSE) && Counter<=HRCC_CLOCK_TIME_OUT)
    {
      Counter++;
    }
   }
   /*Load the Default Values of the PLL                                   */
   else if(RCC_CFGR_SW_PLL)
   {
     /*Timeout mechanism                                                   */
    while(!RCC_GetClock_Status(RCC_CFGR_SW_PLL) && Counter<=HRCC_CLOCK_TIME_OUT)
    {
      Counter++;
    }
   }
   /*if the argument is not valid                                       */
   else
   {
    LocalError=NOK;
   }
   /*The Clock not ready yet                                            */
  if(Counter>=HRCC_CLOCK_TIME_OUT)
  {
    LocalError=NOK;
  }
  return LocalError;
}

/*This function is used to Set the system Clock to one of three arguments */
/*Inputs:                                                                 */
/*          1)RCC_CFGR_SW_HSI                                             */
/*          2)RCC_CFGR_SW_HSE                                             */
/*          3)RCC_CFGR_SW_PLL                                             */
/*Output:                                                                 */
/*          1)OK                                                          */
/*          2)NOK                                                         */


STD_ERROR HRCC_SetSystemClock(u32 RCC_CFGR_SW)
{
  STD_ERROR LocalError=HRCC_SetClockON(RCC_CFGR_SW);
  if (LocalError==OK)
  {
    LocalError=RCC_SetSystemClock(RCC_CFGR_SW);
  }
  return  LocalError;
}

STD_ERROR HRCC_ConfigurePLL(u32 RCC_CFGR_PLLSRC,u32 RCC_CFGR_PLLMUL_MUL,u32 RCC_CFGR_PLLXTPRE )
{
  STD_ERROR LocalError=OK;
  /*Set the system Clock to HSI before changing the Clock to PLL          */
  LocalError=HRCC_SetSystemClock(RCC_CFGR_SW_HSI);

  if(LocalError==OK)
  {
    /*Turn Off the PLL                                                   */
   RCC_SetClock_Status(RCC_CFGR_SW_PLL,RCC_STATUS_OFF);

   /*Turn on the Source Clock of the PLL                                 */
   if(RCC_CFGR_PLLSRC==RCC_CFGR_PLLSRC_HSI_DIV2)
   {
    LocalError=HRCC_SetClockON(RCC_CFGR_SW_HSI);
   }
   else if(RCC_CFGR_PLLSRC==RCC_CFGR_PLLSRC_HSE)
   {
    LocalError=HRCC_SetClockON(RCC_CFGR_SW_HSE);
   }
   else
   {
    LocalError=NOK;
   }
  }
  /*Set the PLLON Bit                                                          */
  if (LocalError==OK)
  {
	  LocalError=RCC_SetPLLSource(RCC_CFGR_PLLSRC);
  }
  if(LocalError==OK && RCC_CFGR_PLLSRC==RCC_CFGR_PLLSRC_HSE)
  {
	  /*Set the PLL Divisor												  */
	  	RCC_SetPLL_Divisor(RCC_CFGR_PLLXTPRE);
  }
  if(LocalError==OK)
  {
	   /*Set the Mul factor                                                  */
	   RCC_SetPLLMULL(RCC_CFGR_PLLMUL_MUL);
  }
  if (LocalError==OK)
    {
     /*turn on the PLL                                                         */
	  RCC_SetClock_Status(CLOCK_PLL,RCC_STATUS_ON);
    }
return LocalError;
}


