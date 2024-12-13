#include "pacman.h"

extern game_t game;
extern cell_t grid[GRID_HEIGHT][GRID_WIDTH];

void pacman_timer_IRQ(){
	// decrement random power pills timer counter
	if(game.pp_spawn_counter != 0){
		game.pp_spawn_counter--;
	}
	else{
		spawn_random_pp(grid, &game);
	}
	// update game timer
	update_game_time(&game);
}