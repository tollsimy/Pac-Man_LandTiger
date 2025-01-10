#include "pacman.h"

#include "../GLCD/AsciiLib.h"
#include "../GLCD/GLCD.h"

extern volatile uint8_t btn_flag;

void draw_game(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	LCD_Clear(Black);
	render_stats(game);
	for(int i=0; i<GRID_HEIGHT; i++){
		for(int j=0; j<GRID_WIDTH; j++){
			switch(grid[i][j]){
				case HOR_WALL:
					render_hor_wall(j,i);
					break;
				case VER_WALL:
					render_ver_wall(j,i);
					break;
				case PILL:
					render_pill(j,i);
					break;
				case POWER_PILL:
					render_power_pill(j,i);
					break;
				case PLAYER:
					render_player(j,i, 0);
					break;
				case GATE:
					render_gate(j,i);
					break;
				case EMPTY:
					break;
			}
		}
	}
};