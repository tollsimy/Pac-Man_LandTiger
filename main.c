#include "include.h"
#include "common.h"

#define RIT_PERIOD_MS 50U

// Private Variables
static volatile uint32_t Sys_Tick = 0;

// Local Functions Prototypes
static void InitSysTick(void);
void SysTick_Handler(void);
void Delay_SysTick(uint32_t SysTicks);

int main (void) {
	// Imperative Inits
	SystemInit();
	InitSysTick();
	
	// Other peripherals Init
	//LED_init();
	joystick_init();
	LCD_Initialization();
	//ADC_init();
	
	BUTTON_init(BUTTON_0, PRIO_3);
	BUTTON_init(BUTTON_1, PRIO_3);
	BUTTON_init(BUTTON_2, PRIO_3);
	
	// RIT WORKS WITH CLOCK = 100MHZ
	// ONE INTERRUPT EVERY 50ms
	init_RIT(RIT_MS_TO_TICKS(RIT_PERIOD_MS)); enable_RIT();
	
	/* TIMER INSTRUCTIONS
	//	init_timer_simplified:
	//		1: which timer
	// 		2: prescaler
	//		3: mr0
	// 		4: mr1
	//		5: configuration of MR0 (interrupt, reset and stop) ( | of TIMER_INTERRUPT_MR, TIMER_RESET_MR, TIMER_STOP_MR)
	//		6: configuration of MR1 (interrupt, reset and stop) ( | of TIMER_INTERRUPT_MR, TIMER_RESET_MR, TIMER_STOP_MR)
	
	// TIMER_0
	// f_timer_hz    = 25000000 Hz = 25MHz
	// prescaler     = 
	// MR            = 
	// f_counter_hz  = f_timer_hz / (prescaler + 1) = 
	// T_timer       = MR / f_counter_hz = 
	// MR = T_timer * f_timer_hz / (prescaler + 1) = 
	
	// USE TIM_MS_TO_TICKS_SIMPLE for default PR (=0) and f_timer_hz values
	// use TIM_MS_TO_TICKS for custom PR and f_timer_hz values
	// max ms value is 170000 (2min 50s) => TIM_MS_TO_TICKS_SIMPLE(170000)=2^31-1
	*/
	// init_timer_simplified(TIMER_0, prescale, mr0, mr1, conf_mr0, conf_mr1); enable_timer(TIMER_0, PRIO_3);
	
	// init_timer_pwm(TIMER_0, 0.5, 170000U); enable_timer(TIMER_0, PRIO_3);
	
	//init_timer_simplified(TIMER_0, 0, TIM_MS_TO_TICKS_SIMPLE(1000), 0, TIMER_INTERRUPT_MR | TIMER_RESET_MR, 0);
	//enable_timer(TIMER_0, PRIO_3);
	
	// power control register
	//execution of wfi or wfe assembly enters Power-Down mode when SLEEPDEEP is on
	LPC_SC->PCON |= 0x1;					// PM0=1
	LPC_SC->PCON &= 0xFFFFFFFFD;	// PM1=0
	
	pacman();

	__ASM("wfi");
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