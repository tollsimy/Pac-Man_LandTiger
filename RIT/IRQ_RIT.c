#include "RIT.h"
#include "../common.h"
#include "../timer/timer.h"
#include "../joystick/joystick.h"
#include "../button_EXINT/button.h"
#include "../adc/adc.h"
#include "../pacman/pacman.h"

#define SHORT_PRESS_COUNT (SHORT_PRESS_MS / RIT_PERIOD_MS)
#define LONG_PRESS_COUNT (LONG_PRESS_MS / RIT_PERIOD_MS)

volatile uint32_t pressed_button_0 = 0;
volatile uint32_t pressed_button_1 = 0;
volatile uint32_t pressed_button_2 = 0;

volatile uint32_t pressed_joystick_up     = 0;
volatile uint32_t pressed_joystick_left   = 0;
volatile uint32_t pressed_joystick_right  = 0;
volatile uint32_t pressed_joystick_down   = 0;
volatile uint32_t pressed_joystick_select = 0;

volatile uint8_t joystick_flag = 0;
volatile uint8_t btn_flag = 0;

extern game_t game;
extern void pause_handler(game_t* game);

void RIT_IRQHandler(void){	
	
	if(game.started && !game.pause){
		// -------------------------------
		// JOYSTICK UP
		// -------------------------------
		
		if(joystick_check_dir(JOYSTICK_UP)){
			pressed_joystick_up++;
			if(pressed_joystick_up == 1) {
				//joystick_flag |= FLAG_JOYSTICK_UP;
				game.player->next_pdir = UP;
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
				game.player->next_pdir = DOWN;
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
				game.player->next_pdir = LEFT;	
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
				game.player->next_pdir = RIGHT;
				//joystick_flag &= ~FLAG_JOYSTICK_RIGHT;
			}
		}
		else pressed_joystick_right = 0;
		
		if(game.player->pdir == STOP){
			game.player->pdir = game.player->next_pdir;
			game.player->next_pdir = STOP;
		}
	}
	
	// -------------------------------
	// PAUSE BTN
	// -------------------------------
	
	if(pressed_button_0 != 0){
		if(LPC_GPIO2->FIOPIN & (1 << 10)){
			if(pressed_button_0 >= SHORT_PRESS_COUNT){
				// short press
				btn_flag |= FLAG_BUTTON_0_SHORT;
				if(game.started){
					game.pause = !game.pause;
					pause_handler(&game);
				}
				btn_flag &= ~FLAG_BUTTON_0_SHORT;
			}
			pressed_button_0 = 0;
			NVIC_EnableIRQ(EINT0_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 20);
		} else {
			if(pressed_button_0 >= LONG_PRESS_COUNT){
				// long press
				btn_flag |= FLAG_BUTTON_0_LONG;
				pressed_button_0 = 0;
			}
			pressed_button_0++;
		}
	}
	
	// -------------------------------
	// START/RESTART BTN
	// -------------------------------
	
	if(pressed_button_1 != 0){
		if(LPC_GPIO2->FIOPIN & (1 << 11)){
			if(pressed_button_1 >= SHORT_PRESS_COUNT){
				// short press
				btn_flag |= FLAG_BUTTON_1_SHORT;
			}
			pressed_button_1 = 0;
			NVIC_EnableIRQ(EINT1_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 22);
		} else {
			if(pressed_button_1 >= LONG_PRESS_COUNT){
				// long press
				btn_flag |= FLAG_BUTTON_1_LONG;
				pressed_button_1 = 0;
			}
			pressed_button_1++;
		}
	}

	// -------------------------------
	// BUTTON 2
	// -------------------------------
	
	if(pressed_button_2 != 0){
		if(LPC_GPIO2->FIOPIN & (1 << 12)){
			if(pressed_button_2 >= SHORT_PRESS_COUNT){
				// short press
				btn_flag |= FLAG_BUTTON_2_SHORT;
			}
			pressed_button_2 = 0;
			NVIC_EnableIRQ(EINT2_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 24);
		} else {
			if(pressed_button_2 >= LONG_PRESS_COUNT){
				// long press
				btn_flag |= FLAG_BUTTON_2_LONG;
				pressed_button_2 = 0;
			}
			pressed_button_2++;
		}
	}
	
	play_melody_note(game.melody.melody, game.melody.length);
	
	LPC_RIT->RICTRL |= 0x1;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
