#include "pacman.h"

void pacman(void){
	// initial states
	state_t state = STATE_RESET;
	game_t game = {
		.lifes = INITIAL_LIFES,
		.pills = INITIAL_PILLS,
		.score = INITIAL_SCORE,
		.time = INITIAL_TIME,
		.player_x = PLAYER_INITIAL_POS_X,
		.player_y = PLAYER_INITIAL_POS_Y,
	};
	cell_t grid[GRID_HEIGHT][GRID_WIDTH] = {
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL}, 
			{HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL, HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL},
			{HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL},
			{HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL},
			{HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL},
			{HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL},
			{HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL},
			{HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL},
			{HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL, HOR_WALL, PILL, PILL, PILL, PILL, HOR_WALL, HOR_WALL, PILL, PILL, PILL, PILL, HOR_WALL, HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, HOR_WALL, EMPTY, EMPTY, HOR_WALL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL},
			{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PILL, EMPTY, EMPTY, EMPTY, HOR_WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, HOR_WALL, EMPTY, EMPTY, EMPTY, PILL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL}, 
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, EMPTY, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL},
			{HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL, HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL},
			{HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL},
			{HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL},
			{HOR_WALL, PILL, PILL, PILL, HOR_WALL, HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL, HOR_WALL, PILL, PILL, PILL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL},
			{HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL, HOR_WALL, PILL, PILL, PILL, PILL, HOR_WALL, HOR_WALL, PILL, PILL, PILL, PILL, HOR_WALL, HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL},
			{HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL},
			{HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, PILL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, PILL, HOR_WALL},
			{HOR_WALL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, HOR_WALL},
			{HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL, HOR_WALL}
	};	
	
	while (1) {
		switch(state){
			case STATE_RESET:
				wait_ready();
				state = STATE_READY;
				break;
			case STATE_READY:
				draw_game(grid,&game);
				state = STATE_PLAY;
				break;
			case STATE_PLAY:
				state = !!(play_game(grid,&game)) ? STATE_WIN : STATE_LOOSE;
				break;
			case STATE_WIN:
				break;
			case STATE_LOOSE:
				break;
		}
	}
}