#include "pacman.h"

extern game_t game;
extern cell_t grid[GRID_HEIGHT][GRID_WIDTH];

static uint8_t counter = 0;

void pacman_fright_IRQ(){
	game.enemy->enemy_fright = 0;
}

void pacman_enemy_respawn_IRQ(){
	// all enemies inside prison can now spawn
	for(int i=0; i<ENEMY_NUM;i++){
		if(game.enemy->enemy_y[i] >= 13 && game.enemy->enemy_y[i] <= 15 &&  game.enemy->enemy_x[i] >= 11 && game.enemy->enemy_x[i] <= 16){
			game.enemy->respawn[i] = 1;
		}
	}
}

void pacman_timer_IRQ(){
	
	move(grid,&game);
	if(!(counter % game.enemy->enemy_delay)){
		calc_enemy_dir(grid, &game);
		move_enemies(grid,&game);
	}
	//TODO: move out all other update_stats from move and move_enemies
	update_stats(grid, &game);
	// 1s
	if(counter == 9){
		// decrement random power pills timer counter
		if(game.pp_spawn_counter != 0){
			game.pp_spawn_counter--;
		}
		else{
			spawn_random_pp(grid, &game);
		}
		// update game timer
		if(update_game_time(&game) % 15 == 0){
			// increment blinky speed
			game.enemy->enemy_delay--;
		}
		counter = 0;
	}
	else{
		counter++;
	}
}