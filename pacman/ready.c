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
					render_hor_wall(i,j);
					break;
				case VER_WALL:
					render_ver_wall(i,j);
					break;
				case PILL:
					render_pill(i,j);
					break;
				case POWER_PILL:
					render_power_pill(i,j);
					break;
				case PLAYER:
					render_player(i,j);
					break;
				case EMPTY:
					break;
			}
		}
	}
};