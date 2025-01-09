#include "include.h"
#include "common.h"

#define RIT_PERIOD_MS 50U

static volatile uint32_t Sys_Tick = 0;

static void InitSysTick(void);
void SysTick_Handler(void);
void Delay_SysTick(uint32_t SysTicks);

int main (void) {
	SystemInit();
	InitSysTick();
	
	joystick_init();
	LCD_Initialization();
	
	BUTTON_init(BUTTON_0, PRIO_3);
	BUTTON_init(BUTTON_1, PRIO_3);
	BUTTON_init(BUTTON_2, PRIO_3);
	
	// RIT WORKS WITH CLOCK = 100MHZ
	// ONE INTERRUPT EVERY 50ms
	init_RIT(RIT_MS_TO_TICKS(RIT_PERIOD_MS)); 
	enable_RIT();
	
	LPC_SC->PCON |= 0x1;					// PM0=1
	LPC_SC->PCON &= 0xFFFFFFFFD;	// PM1=0
	
	pacman();
}

/* Initialize SysTick using CMSIS Core_CM4 function */
static void InitSysTick(void){
	SysTick_Config(SystemFrequency/1000U); /* Configure the SysTick timer */
}
/* SysTick Interrupt Handler */
void SysTick_Handler(void){
	Sys_Tick++; /* increment timer */
}
/* Delay Function based on SysTick Counter */
void Delay_SysTick(uint32_t SysTicks){
	uint32_t DelayTimer_SysTick = Sys_GetTick() + SysTicks; /* Get End Tick */
	while(Sys_GetTick() < DelayTimer_SysTick); 							/* wait for timer */
}
/*Get Current Elapsed Ticks*/
uint32_t Sys_GetTick(void){
	return Sys_Tick;
}