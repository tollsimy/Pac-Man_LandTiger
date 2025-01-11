#include "pacman.h"
#include <stdlib.h>

extern volatile uint8_t btn_flag;
extern CAN_msg CAN_RxMsg;
extern volatile unsigned char CAN2_RX;

static void add_player(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	grid[game->player_y][game->player_x] = PLAYER;
}

static void add_enemies(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	for(int i=0; i<ENEMY_NUM; i++){
		grid[game->enemy_y[i]][game->enemy_x[i]] = ENEMY;
	}
}

// return 1 if game ended, else 0
char update_stats(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	static int prev_score;
	if(grid[game->player_y][game->player_x] == PILL){
		if(game->pills > 0){
			game->pills --;
		}
		game->score += 10;
	} else if(grid[game->player_y][game->player_x] == POWER_PILL) {
		if(game->pills > 0){
			game->power_pills --;
		}
		game->score += 50;
	}
	if(prev_score != game->score && game->score % 1000 == 0 && game->score != 0){
		game->lifes += 1;
	}
	// victory, do not advance anymore when pills finished
	if(game->pills == 0 && game->power_pills == 0){
		game->victory = 1;
	}
	// game over, do not advance anymore when time elapsed
	if(game->time == 0){
		game->victory = 0;
	}
	prev_score = game->score;
	
#ifndef SIMULATOR
	// Send Stats via CAN bus
	CAN_send_stats(game);
	// TODO: test
	// Wait until stats recevied
	while(!CAN2_RX){
		__ASM("wfi");
	};
	CAN2_RX = 0;
#endif
	update_stats_CAN(CAN_RxMsg, game);
	return game->victory == -1 ? 0 : 1;
}

static void move(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	while(game->pause != 1){ // break when pause
		int old_player_x = game->player_x;
		int old_player_y = game->player_y;
		
		int new_player_y = game->player_y;
		int new_player_x = game->player_x;
		
		int angle = 0;
		switch(game->pdir){
			case UP:
				new_player_y--;
				angle = 270;
				break;
			case DOWN:
				new_player_y++;
				angle = 90;
				break;
			case RIGHT:
				new_player_x++;
				angle = 0;
				break;
			case LEFT:
				new_player_x--;
				angle = 180;
				break;
			case STOP:
				break;
		}
		
		// Teleport
		if(new_player_x == GRID_WIDTH){
			new_player_x = 0;
		}
		if(new_player_y == GRID_HEIGHT){
			new_player_y = 0;
		}
		if(new_player_x == -1){
			new_player_x = GRID_WIDTH-1;
		}
		if(new_player_y == -1){
			new_player_y = GRID_HEIGHT-1;
		}
		
		if(grid[new_player_y][new_player_x] != HOR_WALL && grid[new_player_y][new_player_x] != VER_WALL && grid[new_player_y][new_player_x] != GATE) {
			game->player_x = new_player_x;
			game->player_y = new_player_y;

			char victory = update_stats(grid, game);
			
			// render new pos
			grid[game->player_y][game->player_x] = PLAYER;
			grid[old_player_y][old_player_x] = EMPTY;
			render_new_p_pos(old_player_x, old_player_y, game->player_x, game->player_y, angle);
			
			// break after rendering last pos
			if (victory){
				break;
			}
			
			// if user changed dir
			if(game->next_pdir != STOP){
				// try to change dir if no wall in that dir
				new_player_y += game->next_pdir == UP ? -1 : 0;
				new_player_y += game->next_pdir == DOWN ? 1 : 0;
				new_player_x += game->next_pdir == RIGHT ? 1 : 0;
				new_player_x += game->next_pdir == LEFT ? -1 : 0;
				if(grid[new_player_y][new_player_x] != HOR_WALL && grid[new_player_y][new_player_x] != VER_WALL && grid[new_player_y][new_player_x] != GATE) {
					game->pdir = game->next_pdir;
					game->next_pdir = STOP;
				}
			} 
		}
		else {
			game->pdir = STOP;
			break;
		}
		delay_ms(80, TIMER_0);
	}
}

//TODO
static void move_enemies(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	for(int i=0; i<ENEMY_NUM; i++){
		if(game->edir[i] != STOP){
			int old_enemy_x = game->enemy_x[i];
			int old_enemy_y = game->enemy_y[i];
			
			int new_enemy_y = old_enemy_y;
			int new_enemy_x = old_enemy_x;
			switch(game->edir[i]){
				case UP:
					new_enemy_y--;
					break;
				case DOWN:
					new_enemy_y++;
					break;
				case LEFT:
					new_enemy_x--;
					break;
				case RIGHT:
					new_enemy_x++;
					break;
				case STOP:
					return;
			}
			
			if(grid[new_enemy_y][new_enemy_x] == GATE){
				// Gate can be trepassed only from inside prison
				if(old_enemy_y > new_enemy_y){
					game->enemy_x[i] = new_enemy_x;
					game->enemy_y[i] = new_enemy_y - 1; // Jump gate
					
					cell_t prev_cell = grid[game->enemy_y[i]][game->enemy_x[i]];
					grid[game->enemy_y[i]][game->enemy_x[i]] = ENEMY;
					grid[old_enemy_y][old_enemy_x] = EMPTY;
					// TODO: angle
					render_new_e_pos(old_enemy_x, old_enemy_y, game->enemy_x[i], game->enemy_y[i], prev_cell, 0);
				}
				else {
					game->edir[i] = STOP;
				}
			}
			else if(grid[new_enemy_y][new_enemy_x] != HOR_WALL && grid[new_enemy_y][new_enemy_x] != VER_WALL) {
				game->enemy_x[i] = new_enemy_x;
				game->enemy_y[i] = new_enemy_y;
				
				cell_t prev_cell = grid[game->enemy_y[i]][game->enemy_x[i]];
				grid[game->enemy_y[i]][game->enemy_x[i]] = ENEMY;
				grid[old_enemy_y][old_enemy_x] = prev_cell;
				// TODO: angle
				render_new_e_pos(old_enemy_x, old_enemy_y, game->enemy_x[i], game->enemy_y[i], prev_cell, 0);
			}
			else {
				game->edir[i] = STOP;
			}
		}
	}
}

char update_game_time(game_t* game){
	if(!game->pause){
#ifndef SIMULATOR
		// Send stats via CAN bus
		CAN_send_stats(game);
		// TODO: test 
		//Wait until stats recevied
		while(!CAN2_RX){
			__ASM("wfi");
		};
		CAN2_RX = 0;
#endif
		if(game->time > 0){
			game->time -= 1;
		}
		update_time_CAN(CAN_RxMsg, game);
	}
	return game->time;
}

void spawn_random_pp(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	// random power pills
	if(game->power_pills_spawn > 0){
			game->pp_spawn_counter = (rand() % (INITIAL_TIME / (INITIAL_POWER_PILLS - 1)));
			game->power_pills_spawn--;
			game->power_pills++;
			// spawn random power pill
			int rand_y, rand_x;
			do{
				rand_y = rand() % GRID_WIDTH;
				rand_x = rand() % GRID_WIDTH;
			}
			while(grid[rand_y][rand_x] != PILL);
			grid[rand_y][rand_x] = POWER_PILL;
			render_power_pill(rand_x,rand_y);
	}
}

void pause_handler(game_t* game, int pause){
	if(pause){
		game->pause = 1;
		render_pause(1);
	} else {
		game->pause = 0;
		render_pause(0);
	}
}

uint8_t play_game(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	
	// Countdown 3s
	int8_t count = 3;
	while(count >= 0){
		delay_ms(1000, TIMER_0);
		render_countdown(count);
		count--;
	}
	clear_countdown();
	
	uint32_t game_timer = 100;
	struct timer_configuration tc = {
		.timer_n = TIMER_1,
		.prescale = 0,
		.mr0 = TIM_MS_TO_TICKS_SIMPLE(game_timer),
		.configuration_mr0 = TIMER_INTERRUPT_MR | TIMER_RESET_MR,
	};
	init_timer(&tc);
	
	// random power pills seed init
	srand(Sys_GetTick());
	game->pp_spawn_counter = (rand() % (INITIAL_TIME / (INITIAL_POWER_PILLS - 1)));
	
	add_player(grid, game);
	add_enemies(grid, game);
	render_player(game->player_x, game->player_y, 0);
	
	enable_timer(tc.timer_n, PRIO_3);
	
	game->started = 1;
		
	while(game->victory == -1){
		__ASM("wfi");
		if(game->pdir != STOP){
			move(grid, game);
		}
		if(game->pills == 0 && game->power_pills == 0){
			game->victory = 1;
		}
		if(game->time == 0){
			game->victory = 0;
		}
	}
	disable_timer(1);
	return game->victory;
}

void win(){
	GUI_Text(100, 100, (uint8_t*) "Victory!", Yellow, Black);
	GUI_Text(80, 120, (uint8_t*) "KEY1 to restart!", Yellow, Black);
	btn_flag &= ~FLAG_BUTTON_1;
	while(!(btn_flag & FLAG_BUTTON_1)){ __ASM("wfi"); };
	btn_flag &= ~FLAG_BUTTON_1;
}

void loose(){
	GUI_Text(100, 100, (uint8_t*) "Game Over!", Yellow, Black);
	GUI_Text(80, 120, (uint8_t*) "KEY1 to restart!", Yellow, Black);
	btn_flag &= ~FLAG_BUTTON_1;
	while(!(btn_flag & FLAG_BUTTON_1)){ __ASM("wfi"); };
	btn_flag &= ~FLAG_BUTTON_1;
}