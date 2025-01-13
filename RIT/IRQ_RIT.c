#include "RIT.h"
#include "../common.h"
#include "../timer/timer.h"
#include "../joystick/joystick.h"
#include "../button_EXINT/button.h"
#include "../adc/adc.h"
#include "../pacman/pacman.h"

volatile uint32_t pressed_button_0 = 0;
volatile uint32_t pressed_button_1 = 0;
volatile uint32_t pressed_button_2 = 0;

uint32_t pressed_joystick_up     = 0;
uint32_t pressed_joystick_left   = 0;
uint32_t pressed_joystick_right  = 0;
uint32_t pressed_joystick_down   = 0;
uint32_t pressed_joystick_select = 0;

volatile uint8_t joystick_flag = 0;
volatile uint8_t btn_flag = 0;

extern volatile uint8_t PLAY_SONG;
extern game_t game;
extern void pause_handler(game_t* game, int pause_val);

void RIT_IRQHandler(void){	
	
	if(game.started && !game.pause){
		// -------------------------------
		// JOYSTICK UP
		// -------------------------------
		
		if(joystick_check_dir(JOYSTICK_UP)){
			pressed_joystick_up++;
			if(pressed_joystick_up == 1) {
				//joystick_flag |= FLAG_JOYSTICK_UP;
				game.next_pdir = UP;
				//joystick_flag &= ~FLAG_JOYSTICK_UP;
			}
		}
		else pressed_joystick_up = 0;
		
		// -------------------------------
		// JOYSTICK DOWN
		// -------------------------------
		
		if(joystick_check_dir(JOYSTICK_DOWN)){
			pressed_joystick_down++;
			if(pressed_joystick_down == 1) {
				//joystick_flag |= FLAG_JOYSTICK_DOWN;
				game.next_pdir = DOWN;
				//joystick_flag &= ~FLAG_JOYSTICK_DOWN;
			}
		}
		else pressed_joystick_down = 0;
		
		// -------------------------------
		// JOYSTICK LEFT
		// -------------------------------
		
		if(joystick_check_dir(JOYSTICK_LEFT)){
			pressed_joystick_left++;
			if(pressed_joystick_left == 1) {
				//joystick_flag |= FLAG_JOYSTICK_LEFT;
				game.next_pdir = LEFT;	
				//joystick_flag &= ~FLAG_JOYSTICK_LEFT;
			}
		}
		else pressed_joystick_left = 0;
		
		// -------------------------------
		// JOYSTICK RIGHT
		// -------------------------------
		
		if(joystick_check_dir(JOYSTICK_RIGHT)){
			pressed_joystick_right++;
			if(pressed_joystick_right == 1) {
				//joystick_flag |= FLAG_JOYSTICK_RIGHT;
				game.next_pdir = RIGHT;
				//joystick_flag &= ~FLAG_JOYSTICK_RIGHT;
			}
		}
		else pressed_joystick_right = 0;
		
		if(game.pdir == STOP){
			game.pdir = game.next_pdir;
			game.next_pdir = STOP;
		}
	}
	
	// -------------------------------
	// PAUSE BTN
	// -------------------------------
	
	if(pressed_button_0 != 0){
		pressed_button_0++;
		if(LPC_GPIO2->FIOPIN & (1 << 10)){
			btn_flag |= FLAG_BUTTON_0;
			
			if(game.started){
				static int toggle_pause = 1;
				pause_handler(&game, toggle_pause);
				toggle_pause = !toggle_pause;
			}
			btn_flag &= ~FLAG_BUTTON_0;
			
			pressed_button_0 = 0;
			NVIC_EnableIRQ(EINT0_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 20);
		}
	}
	
	// -------------------------------
	// START/RESTART BTN
	// -------------------------------
	
	if(pressed_button_1 != 0){
			pressed_button_1++;
			if(LPC_GPIO2->FIOPIN & (1 << 11)){
				btn_flag |= FLAG_BUTTON_1;
				
				pressed_button_1 = 0;
				NVIC_EnableIRQ(EINT1_IRQn);
				LPC_PINCON->PINSEL4 |= (1 << 22);
			}
	}

	// -------------------------------
	// BUTTON 2
	// -------------------------------
	
	if(pressed_button_2 != 0){
			pressed_button_2++;
			if(LPC_GPIO2->FIOPIN & (1 << 12)){
				btn_flag |= FLAG_BUTTON_2;
				
				pressed_button_2 = 0;
				NVIC_EnableIRQ(EINT2_IRQn);
				LPC_PINCON->PINSEL4 |= (1 << 24);
			}
	}
	
	if(PLAY_SONG == 1){
		play_melody_note();
	}
	
	LPC_RIT->RICTRL |= 0x1;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
