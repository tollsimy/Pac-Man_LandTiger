#include "music.h"
#include "../timer/timer.h"

void playNote(NOTE note)
{
	if(note.freq != pause)
	{
		reset_timer(1);
		init_timer_simplified(TIMER_1, 0, 0, note.freq, 0, TIMER_INTERRUPT_MR | TIMER_RESET_MR);
		enable_timer(1, PRIO_1);
	}
	reset_timer(2);
	init_timer_simplified(TIMER_2, 0, 0, note.duration, 0, TIMER_RESET_MR | TIMER_STOP_MR | TIMER_INTERRUPT_MR);
	enable_timer(2, PRIO_1);
}

char isNotePlaying(void)
{
	return ((LPC_TIM1->TCR != 0) || (LPC_TIM2->TCR != 0));
}
