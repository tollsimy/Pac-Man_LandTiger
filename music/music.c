#include "music.h"
#include "../timer/timer.h"
#include "../common.h"

volatile double VOLUME = 0.7 * AMPLIFIER;

void playNote(NOTE note)
{
	if(note.freq != REST)
	{
		reset_timer(2);
		init_timer_simplified(TIMER_2, 0, 0, note.freq, 0, TIMER_INTERRUPT_MR | TIMER_RESET_MR);
		enable_timer(2, PRIO_0);
	}
	reset_timer(3);
	init_timer_simplified(TIMER_3, 0, 0, note.duration, 0, TIMER_RESET_MR | TIMER_STOP_MR | TIMER_INTERRUPT_MR);
	enable_timer(3, PRIO_0);
}

char isNotePlaying(void)
{
	return ((LPC_TIM2->TCR != 0) || (LPC_TIM3->TCR != 0));
}
