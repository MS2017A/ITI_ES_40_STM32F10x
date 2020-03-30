#ifndef SCHEDULER_CONFIG_H_
#define SCHEDULER_CONFIG_H_


/* Here you can define the number of the Tasks here*/
#define Tasks_Num	1



/* Here you can define the tick of the system in ms */
#define TICK_MS		2




typedef struct{

	Task_t* 			Task;
	uint32_t			First_Delay;

}SysTasks_t;

extern SysTasks_t	SysTasks[Tasks_Num];

#endif
