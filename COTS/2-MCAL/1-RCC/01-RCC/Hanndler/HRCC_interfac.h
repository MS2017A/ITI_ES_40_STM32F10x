/************************************************************
Author :Mohamed Ibrahem
Version:v01
Date:16 March 2020
*************************************************************/
#ifndef HRCC_INTERFACE_H
#define HRCC_INTERFACE_H



STD_ERROR HRCC_SetSystemClock(u32 RCC_CFGR_SW);
STD_ERROR HRCC_ConfigurePLL(u32 RCC_CFGR_PLLSRC,u32 RCC_CFGR_PLLMUL_MUL,u32 RCC_CFGR_PLLXTPRE);














#endif
