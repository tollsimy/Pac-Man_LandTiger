#include "pacman.h"
#include <stdlib.h>

extern volatile uint8_t btn_flag;
extern CAN_msg CAN_RxMsg;
extern volatile unsigned char CAN2_RX;

static void add_player(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	grid[game->player->player_y][game->player->player_x] = PLAYER;
}

static void add_enemies(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	for(int i=0; i<ENEMY_NUM; i++){
		grid[game->enemy->enemy_y[i]][game->enemy->enemy_x[i]] = ENEMY;
	}
}

static void start_fright_timer(){
	uint32_t timer = 10000;
	struct timer_configuration tc = {
		.timer_n = TIMER_0,
		.prescale = 0,
		.mr0 = 0,
		.configuration_mr0 = 0,
		.mr1 = TIM_MS_TO_TICKS_SIMPLE(timer),
		.configuration_mr1 = TIMER_INTERRUPT_MR | TIMER_RESET_MR | TIMER_STOP_MR,
		.mr2 = 0,
		.configuration_mr2 = 0,
	};
	init_timer(&tc);
	enable_timer(0, PRIO_2);
}

// return 1 if game ended, else 0
char update_stats(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	static int prev_score;
	if(grid[game->player->player_y][game->player->player_x] == PILL){
		if(game->pills > 0){
			game->pills --;
		}
		game->score += 10;
	} else if(grid[game->player->player_y][game->player->player_x] == POWER_PILL) {
		if(game->pills > 0){
			game->power_pills --;
		}
		game->score += 50;
		game->melody.melody = MELODY_P_PILL;
		game->melody.length = sizeof(MELODY_P_PILL)/sizeof(MELODY_P_PILL[0]);
		enable_melody();
		for(int i=0; i<ENEMY_NUM;i++){
			if(game->enemy->respawn[i]){
				render_enemy(game->enemy->enemy_x[i], game->enemy->enemy_y[i], 0, game->enemy->enemy_fright);
			}
		}
		game->enemy->enemy_fright = 1;
		start_fright_timer();
	}
	
	for(int i=0; i<ENEMY_NUM; i++){
		if(check_collision(game, i)){
			if(!game->enemy->enemy_fright){
				if(game->lifes > 1){
					game->lifes--;
					respawn_pacman(grid, game);
					respawn_enemy(grid, game, i);
					game->melody.melody = MELODY_DEATH;
					game->melody.length = sizeof(MELODY_DEATH)/sizeof(MELODY_DEATH[0]);
					enable_melody();
				}
				else if (game->lifes == 1){
					game->lifes--;
#ifndef SIMULATOR
					CAN_send_stats(game);
					while(!CAN2_RX){
						__ASM("wfi");
					};
					CAN2_RX = 0;
#endif
					update_stats_CAN(CAN_RxMsg, game);
					game->victory = 0;
				}
			} else {
				game->score += 100;
				game->enemy->respawn[i] = 0;
				respawn_enemy(grid,game, i);
				game->melody.melody = MELODY_ENEMY_DEATH;
				game->melody.length = sizeof(MELODY_ENEMY_DEATH)/sizeof(MELODY_ENEMY_DEATH[0]);
				enable_melody();
			}
		}
	}

	if (prev_score / 1000 < game->score / 1000) {
		game->lifes += (game->score / 1000) - (prev_score / 1000);
		game->melody.melody = MELODY_LIFE;
		game->melody.length = sizeof(MELODY_LIFE)/sizeof(MELODY_LIFE[0]);
		enable_melody();
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
	// Wait until stats recevied
	while(!CAN2_RX){
		__ASM("wfi");
	};
	CAN2_RX = 0;
#endif
	update_stats_CAN(CAN_RxMsg, game);
	return game->victory == -1 ? 0 : 1;
}

void move(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	if(game->pause != 1){ // break when pause
		int old_player_x = game->player->player_x;
		int old_player_y = game->player->player_y;
		
		int new_player_y = game->player->player_y;
		int new_player_x = game->player->player_x;
		
		int angle = 0;
		switch(game->player->pdir){
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
		
		// break if not moving
		if(new_player_x == old_player_x && new_player_y == old_player_y){
			return;
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
			game->player->player_x = new_player_x;
			game->player->player_y = new_player_y;

			char victory = update_stats(grid, game);
			
			// render new pos
			grid[game->player->player_y][game->player->player_x] = PLAYER;
			grid[old_player_y][old_player_x] = EMPTY;
			render_new_p_pos(old_player_x, old_player_y, game->player->player_x, game->player->player_y, angle);
			
			// break after rendering last pos
			if (victory){
				return;
			}
			
			// if user changed dir
			if(game->player->next_pdir != STOP){
				// try to change dir if no wall in that dir
				new_player_y += game->player->next_pdir == UP ? -1 : 0;
				new_player_y += game->player->next_pdir == DOWN ? 1 : 0;
				new_player_x += game->player->next_pdir == RIGHT ? 1 : 0;
				new_player_x += game->player->next_pdir == LEFT ? -1 : 0;
				if(grid[new_player_y][new_player_x] != HOR_WALL && grid[new_player_y][new_player_x] != VER_WALL && grid[new_player_y][new_player_x] != GATE) {
					game->player->pdir = game->player->next_pdir;
					game->player->next_pdir = STOP;
				}
			} 
		}
		else {
			game->player->pdir = STOP;
			return;
		}
	}
}

void move_enemies(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	for(int i=0; i<ENEMY_NUM; i++){
		if(!game->pause){
			// TODO: bring check out
			if(game->enemy->edir[i] != STOP){
				int old_enemy_x = game->enemy->enemy_x[i];
				int old_enemy_y = game->enemy->enemy_y[i];
				
				int new_enemy_y = old_enemy_y;
				int new_enemy_x = old_enemy_x;
				switch(game->enemy->edir[i]){
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
						game->enemy->enemy_x[i] = new_enemy_x;
						game->enemy->enemy_y[i] = new_enemy_y - 1; // Jump gate
						
						cell_t prev_cell = grid[game->enemy->enemy_y[i]][game->enemy->enemy_x[i]];
						grid[game->enemy->enemy_y[i]][game->enemy->enemy_x[i]] = ENEMY;
						grid[old_enemy_y][old_enemy_x] = EMPTY;
						render_new_e_pos(old_enemy_x, old_enemy_y, game->enemy->enemy_x[i], game->enemy->enemy_y[i], prev_cell, 0, game->enemy->enemy_fright);
					}
					else {
						game->enemy->edir[i] = STOP;
					}
				}
				else if(grid[new_enemy_y][new_enemy_x] != HOR_WALL && grid[new_enemy_y][new_enemy_x] != VER_WALL) {
					game->enemy->enemy_x[i] = new_enemy_x;
					game->enemy->enemy_y[i] = new_enemy_y;
					
					if(update_stats(grid, game)){
						break;
					}
					
					cell_t prev_cell = grid[game->enemy->enemy_y[i]][game->enemy->enemy_x[i]];
					grid[game->enemy->enemy_y[i]][game->enemy->enemy_x[i]] = ENEMY;
					grid[old_enemy_y][old_enemy_x] = prev_cell;
					render_new_e_pos(old_enemy_x, old_enemy_y, game->enemy->enemy_x[i], game->enemy->enemy_y[i], prev_cell, 0, game->enemy->enemy_fright);
				}
				game->enemy->edir[i] = STOP;
			}
		}
	}
}

uint8_t check_collision(game_t* game, uint8_t enemy_num){
	uint8_t collision = 0;
	if(game->player->player_x == game->enemy->enemy_x[enemy_num] && game->player->player_y == game->enemy->enemy_y[enemy_num]){
		collision++;
	}
	return collision;
}

void respawn_pacman(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	grid[game->player->player_y][game->player->player_x] = EMPTY;
	int old_player_x = game->player->player_x;
	int old_player_y = game->player->player_y;
	game->player->player_x = PLAYER_INITIAL_POS_X;
	game->player->player_y = PLAYER_INITIAL_POS_Y;
	grid[game->player->player_y][game->player->player_x] = PLAYER;
	render_new_p_pos(old_player_x, old_player_y, game->player->player_x, game->player->player_y, 0);
}

void respawn_enemy(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game, uint8_t enemy_num){
	game->enemy->enemy_x[enemy_num] = INITIAL_ENEMY_X;
	game->enemy->enemy_y[enemy_num] = INITIAL_ENEMY_Y;
	// reset frightened mode
	game->enemy->enemy_fright = 0;

	// timer 3s
	uint32_t timer = 3000;
	struct timer_configuration tc = {
		.timer_n = TIMER_0,
		.prescale = 0,
		.mr0 = 0,
		.configuration_mr0 = 0,
		.mr1 = 0,
		.configuration_mr1 = 0,
		.mr2 = TIM_MS_TO_TICKS_SIMPLE(timer),
		.configuration_mr2 = TIMER_INTERRUPT_MR | TIMER_RESET_MR | TIMER_STOP_MR,
	};
	init_timer(&tc);
	reset_timer(0);
	enable_timer(0, PRIO_2);
}

char update_game_time(game_t* game){
	if(!game->pause){
		if(game->time > 0){
			game->time -= 1;
		}
#ifndef SIMULATOR
		// Send stats via CAN bus
		CAN_send_stats(game);
		//Wait until stats recevied
		while(!CAN2_RX){
			__ASM("wfi");
		};
		CAN2_RX = 0;
#endif
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

void pause_handler(game_t* game){
	if(game->pause){
		render_pause(game->pause);
	} else {
		render_pause(game->pause);
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
		.mr1 = 0,
		.configuration_mr1 = 0,
		.mr2 = 0,
		.configuration_mr2 = 0,
	};
	init_timer(&tc);
	
	// random power pills seed init
	srand(Sys_GetTick());
	game->pp_spawn_counter = (rand() % (INITIAL_TIME / (INITIAL_POWER_PILLS - 1)));
	
	add_player(grid, game);
	add_enemies(grid, game);
	render_player(game->player->player_x, game->player->player_y, 0);
	for(int i=0; i<ENEMY_NUM; i++){
		render_enemy(game->enemy->enemy_x[i], game->enemy->enemy_y[i], 0, game->enemy->enemy_fright);
	}
	
	enable_timer(tc.timer_n, PRIO_3);
	
	game->started = 1;
	
	while(game->victory == -1){
		__ASM("wfi");
	}
	disable_timer(1);
	return game->victory;
}

void win(game_t* game){
	GUI_Text(100, 100, (uint8_t*) "Victory!", Yellow, Black);
	GUI_Text(80, 120, (uint8_t*) "KEY1 to restart!", Yellow, Black);
	game->melody.melody = MELODY_WIN;
	game->melody.length = sizeof(MELODY_WIN)/sizeof(MELODY_WIN[0]);
	enable_melody();
	btn_flag &= ~FLAG_BUTTON_1;
	while(!(btn_flag & FLAG_BUTTON_1)){ __ASM("wfi"); };
	btn_flag &= ~FLAG_BUTTON_1;
}

void lose(game_t* game){
	GUI_Text(100, 100, (uint8_t*) "Game Over!", Yellow, Black);
	GUI_Text(80, 120, (uint8_t*) "KEY1 to restart!", Yellow, Black);
	game->melody.melody = MELODY_LOSE;
	game->melody.length = sizeof(MELODY_LOSE)/sizeof(MELODY_LOSE[0]);
	enable_melody();
	btn_flag &= ~FLAG_BUTTON_1;
	while(!(btn_flag & FLAG_BUTTON_1)){ __ASM("wfi"); };
	btn_flag &= ~FLAG_BUTTON_1;
}