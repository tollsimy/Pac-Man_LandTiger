#ifndef __COMMON_H
#define __COMMON_H

#include "inttypes.h"

/*SystemFrequency defined in file system_LPC17xx.c initialized during SystemInit()*/
extern uint32_t SystemFrequency;

//IRQ priorities
#define PRIO_0 0U
#define PRIO_1 1U
#define PRIO_2 2U
#define PRIO_3 3U
#define PRIO_4 4U
#define PRIO_5 5U

// Get Current Ticks
uint32_t Sys_GetTick(void);
/******************************************************************************
**                            End Of File
******************************************************************************/

#endif // __COMMON_H
