#ifndef _SCHEDULER_INTERFACE_H_
#define _SCHEDULER_INTERFACE_H_


#define TASK_RUNNING_STATE		(1)
#define TASK_SUSPENDED_STATE	(0)

uint8_t OSTickFlag;

typedef void (*Task_Runnable_t)(void);

typedef struct{

	Task_Runnable_t		Runnable;
	uint32_t			Periodicity_ms;
	uint8_t				State;

}Task_t;


/*====================================================== Prototypes of APIs =======================================================*/
extern void Scheduler_Init(void);
extern void Scheduler_Start(void);
extern uint8_t Scheduler_GetTick_ms(void);


#endif
