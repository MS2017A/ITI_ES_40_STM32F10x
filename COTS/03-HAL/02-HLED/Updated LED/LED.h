#ifndef LED_H_
#define LED_H_

#include"STD_TYPES.h"


typedef enum 
{
  LED_Name1		,
  LED_Name2     ,
  LED_Name3     ,
  LED_Name4     ,
  LED_Name5     ,
  LED_Name6     ,
  LED_Name7     ,
  LED_Name8     ,
  LED_Name9     ,
  LED_Name10    ,
  LED_Name11    ,
  LED_Name12    ,
  LED_Name13    ,
  LED_Name14    ,
  LED_Name15    ,
  LED_Name16    ,
  LED_Name17    ,
  LED_Name18    ,
  LED_Name19    ,
  LED_Name20    ,
  LED_Name21    ,
  LED_Name22    ,
  LED_Name23    ,
  LED_Name24    ,
  LED_Name25    ,
  LED_Name26    ,
  LED_Name27    ,
  LED_Name28    ,
  LED_Name29    ,
  LED_Name30    ,
  LED_Name31    ,
  LED_Name32
  
}LED_Names;



typedef struct
{
	u8 LED_Name;
	u32 LED_Pin;
	void * LED_Port;
	u32 LED_ON;
	u32 LED_OFF;
	u32 LED_MODE;
}LED_t;



STD_ERROR LED_Init(u32 LED_Name);
STD_ERROR LED_SetLED_ON(u32 LED_Name);
STD_ERROR LED_SetLED_OFF(u32 LED_Name);



#endif /* LED_H_ */
