#include "pacman.h"
#include <stdlib.h>

extern game_t game;
extern cell_t grid[GRID_HEIGHT][GRID_WIDTH];

static uint8_t counter = 0;

typedef struct {
	int x;
	int y;
} Vec;

static dir_t dir_to_pacman(game_t* game, cell_t grid[GRID_HEIGHT][GRID_WIDTH], int enemy_num){
	dir_t dirs[4] = {STOP, STOP, STOP, STOP}; // Ordered dirs with priority
	int diff_x = game->enemy_x[enemy_num] - game->player_x;
	int diff_y = game->enemy_y[enemy_num] - game->player_y;
	
	if(diff_x != 0 || diff_y != 0){
		if(diff_x*diff_x > diff_y*diff_y){
			// left - right
			dirs[0] = diff_x > 0 ? LEFT : RIGHT;
			dirs[3] = diff_x > 0 ? RIGHT : LEFT;
			dirs[1] = diff_y > 0 ? UP : DOWN;
			dirs[2] = diff_y > 0 ? DOWN : UP;
		} else if (diff_y*diff_y > diff_x*diff_x) {
			// up - down
			dirs[0] = diff_y > 0 ? UP : DOWN;
			dirs[3] = diff_y > 0 ? DOWN : UP; 
			dirs[1] = diff_x > 0 ? LEFT : RIGHT;
			dirs[2] = diff_x > 0 ? RIGHT : LEFT;
		} else {
			// random choice up - down / left - right
			if(rand() % 1){
				dirs[0] = diff_x > 0 ? LEFT : RIGHT;
				dirs[3] = diff_x > 0 ? RIGHT : LEFT;
				dirs[1] = diff_y > 0 ? UP : DOWN;
				dirs[2] = diff_y > 0 ? DOWN : UP;
			} else {
				dirs[0] = diff_x > 0 ? LEFT : RIGHT;
				dirs[3] = diff_x > 0 ? RIGHT : LEFT;
				dirs[1] = diff_y > 0 ? UP : DOWN;
				dirs[2] = diff_y > 0 ? DOWN : UP;
			}
		}
	}
	
	int count = 0;
	cell_t next_cell;
	dir_t dir = STOP;
	do {
		dir = dirs[count];
		int new_x = game->enemy_x[enemy_num] + (dir == RIGHT ? 1 : 0) + (dir == LEFT ? -1 : 0);
		int new_y = game->enemy_y[enemy_num] + (dir == UP ? -1 : 0) + (dir == DOWN ? +1 : 0);
		next_cell = grid[new_y][new_x];
		count++;
	} while (next_cell == HOR_WALL || next_cell == VER_WALL || next_cell == GATE);

	return dir;
}

static void calc_enemy_dir(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	dir_t dir = STOP;
	for(int i=0; i<ENEMY_NUM; i++){
		// if still inside prison
		if(game->enemy_y[i] >= 13 && game->enemy_y[i] <= 15 &&  game->enemy_x[i] >= 11 && game->enemy_x[i] <= 16){
			// Just go up since spawn point static
			dir = UP;
		}
		else {
			dir = dir_to_pacman(game, grid, i);
		}
		game->edir[i] = dir;
	}
}

void pacman_timer_IRQ(){
	//calc_enemy_dir(grid, &game);
	
	if(counter == 9){
		// decrement random power pills timer counter
		if(game.pp_spawn_counter != 0){
			game.pp_spawn_counter--;
		}
		else{
			spawn_random_pp(grid, &game);
		}
		// update game timer
		update_game_time(&game);
		counter = 0;
	}
	else{
		counter++;
	}
}