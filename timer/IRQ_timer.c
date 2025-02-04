#include "timer.h"
#include "../common.h"
#include "../pacman/pacman.h"

extern void pacman_timer_IRQ();
extern void pacman_fright_IRQ();
extern void pacman_enemy_respawn_IRQ();

extern volatile double VOLUME;

/*
* NOTE: Repetitive timers only in MR0!
*/

void timer0_callback(uint8_t irq_src){
	if(irq_src & IR_MR0) { // mr0
		
	} else if(irq_src & IR_MR1) { // mr1
		pacman_fright_IRQ();
	} else if(irq_src & IR_MR2) { // mr2
		pacman_enemy_respawn_IRQ();
	} else if(irq_src & IR_MR3) { // mr3

	}
}

void timer1_callback (uint8_t irq_src){
	if(irq_src & IR_MR0) { // mr0
		// Used only in game play state
		pacman_timer_IRQ();
	} else if(irq_src & IR_MR1) { // mr1
		
	} else if(irq_src & IR_MR2) { // mr2
		
	} else if(irq_src & IR_MR3) { // mr3
	
	}
}

uint16_t SinTable[45] =
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

int sineticks=0;
void timer2_callback(uint8_t irq_src){
	if(irq_src & IR_MR0) { // mr0

	} else if(irq_src & IR_MR1) { // mr1
		/* DAC management */
		LPC_DAC->DACR = ((int)(SinTable[sineticks] * VOLUME)) << 6;
		sineticks++;
		if(sineticks==45) sineticks=0;
	} else if(irq_src & IR_MR2) { // mr2
		
	} else if(irq_src & IR_MR3) { // mr3

	}
}

void timer3_callback(uint8_t irq_src){
	if(irq_src & IR_MR0) { // mr0
		
	} else if(irq_src & IR_MR1) { // mr1
		disable_timer(2);
	} else if(irq_src & IR_MR2) { // mr2
		
	} else if(irq_src & IR_MR3) { // mr3
	
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/

