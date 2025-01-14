#include "pacman.h"

extern game_t game;
extern cell_t grid[GRID_HEIGHT][GRID_WIDTH];

static uint8_t counter = 0;

void pacman_fright_IRQ(){
	game.enemy->enemy_fright = 0;
}

void pacman_timer_IRQ(){
	move(grid,&game);
	if(!(counter % game.enemy->enemy_delay)){
		calc_enemy_dir(grid, &game);
		move_enemies(grid,&game);
	}
	for(int i=0; i<ENEMY_NUM; i++){
		if(check_collision(&game, i)){
			if(!game.enemy->enemy_fright){
				if(game.lifes > 1){
					game.lifes--;
					respawn_pacman(grid, &game);
				}
				else{
					game.victory = 0;
				}
			} else {
				game.score += 100;
				game.enemy->enemy_x[i] = INITIAL_ENEMY_X;
				game.enemy->enemy_y[i] = INITIAL_ENEMY_Y;
				// reset frightened mode
				game.enemy->enemy_fright = 0;
			}
		}
	}
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