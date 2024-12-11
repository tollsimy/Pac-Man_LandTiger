#include "pacman.h"

extern uint8_t joystick_flag;
extern uint8_t btn_flag;

uint8_t play_game(enum cell_t grid[GRID_HEIGHT][GRID_WIDTH], struct game_t* game){
		// Joystick cmd, flags set at first edge
		if(joystick_flag & FLAG_JOYSTICK_UP) {
			joystick_flag &= ~FLAG_JOYSTICK_UP;
		}
		if(joystick_flag & FLAG_JOYSTICK_DOWN) {
			joystick_flag &= ~FLAG_JOYSTICK_DOWN;
		}
		if(joystick_flag & FLAG_JOYSTICK_LEFT) {
			joystick_flag &= ~FLAG_JOYSTICK_LEFT;
		}
		if(joystick_flag & FLAG_JOYSTICK_RIGHT) {
			joystick_flag &= ~FLAG_JOYSTICK_RIGHT;
		}
		if(joystick_flag & FLAG_JOYSTICK_SELECT) {
			joystick_flag &= ~FLAG_JOYSTICK_SELECT;
		}
		// Buttons, flags set when released
		if(btn_flag & FLAG_BUTTON_0) {
			btn_flag &= ~FLAG_BUTTON_0;
		}
		if(btn_flag & FLAG_BUTTON_1) {
			btn_flag &= ~FLAG_BUTTON_1;
		}
		if(btn_flag & FLAG_BUTTON_2) {
			btn_flag &= ~FLAG_BUTTON_2;
		}
		return 1;
}
