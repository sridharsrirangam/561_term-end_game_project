#ifndef TASKS_H
#define TASKS_H

#include <RTL.h>
#include "TFT_LCD.h"

#define TASK_READ_TS_PERIOD_TICKS (20)  // 1 tick/ms
#define TASK_READ_ACCELEROMETER_PERIOD_TICKS (100)  // 1 tick/ms
#define TASK_SOUND_PERIOD_TICKS (100)  // 1 tick/ms
#define TASK_UPDATE_SCREEN_PERIOD_TICKS (100)

// Debug outputs on Port B
#define DEBUG_T0_POS 	0		// J10 pin 2
#define DEBUG_T1_POS 	1 	// J10 pin 4
#define DEBUG_T2_POS 2 	// J10 pin 6
#define DEBUG_T3_POS 	3		// J10 pin 8

 void	Init_Debug_Signals(void);

#define EV_PLAYSOUND (1) 
#define EV_SOUND_ON (2)
#define EV_SOUND_OFF (4)

#define EV_REFILL_SOUND (1)

#define WAIT_FOREVER (0xffff)

 __task void Task_Init(void);
 __task void Task_Read_TS(void);
 __task void Task_Read_Accelerometer(void);
 __task void Task_Update_Screen(void);
 __task void Task_GameStats(void);
 __task void Task_CPUStats(void);
 
extern OS_TID t_Read_TS, t_Read_Accelerometer, t_Sound_Manager, t_US, t_Refill_Sound_Buffer;

 
// Game Constants
#define PADDLE_WIDTH (40)
#define PADDLE_HEIGHT (15)
#define PADDLE_Y_POS (TFT_HEIGHT-4-PADDLE_HEIGHT)

// constants for coin dimensions
#define COIN_RADIUS (10)

// Event Flags for Update Screen
#define EV_SCORE_UPDATE (uint16_t)(0x0001)
#define EV_LIFE_UPDATE (uint16_t)(0x0002)
#define EV_CPU_STAT_DISP (uint16_t)(0x0004)
#define EV_CPU_STAT_DONE (uint16_t)(0x0008)
#define EV_CPU_STAT_RELEASE (uin16_t)(0x0001)

//Event flag to trigger CPU Stat task. Will be set by read_TS task
#define EV_CPU_STAT_UPDATE (uint16_t)(0x0001)
//Event flag for Read TouchScreen
#define EV_TOUCH_DETECTED (uint16_t)(0x0001)
#define EV_END_CPU_STAT		(uint16_t)(0x0002)

//Event flag for Read_TS
#define EV_RECD_CPU_STAT (uint16_t)(0x0001)


#endif // TASKS_H
