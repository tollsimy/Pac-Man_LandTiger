#include "pacman.h"
#include <stdlib.h>

extern volatile uint8_t joystick_flag;
extern volatile uint8_t btn_flag;

static void add_player(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	grid[game->player_y][game->player_x] = PLAYER;
}

static void move(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	char old_joystick_flag = joystick_flag;
	char old_btn_flag = btn_flag;
	while(old_joystick_flag == joystick_flag && old_btn_flag == btn_flag){ // break when new input from user
		int old_player_x = game->player_x;
		int old_player_y = game->player_y;
		
		int new_player_y = game->player_y;
		int new_player_x = game->player_x;
		new_player_y += game->dir == UP ? -1 : 0;
		new_player_y += game->dir == DOWN ? 1 : 0;
		new_player_x += game->dir == RIGHT ? 1 : 0;
		new_player_x += game->dir == LEFT ? -1 : 0;
		
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
			grid[game->player_y][game->player_x] = PLAYER;
			grid[old_player_y][old_player_x] = EMPTY;
			render_new_p_pos(old_player_x, old_player_y, game->player_x, game->player_y);
			
			if(game->score % 1000 == 0 && game->score != 0){
				game->lifes += 1;
			}
			update_stats(game);
			// victory, do not advance anymore when pills finished
			if(game->pills == 0 && game->power_pills == 0){
				game->victory = 1;
				break;
			}
			// game over, do not advance anymore when time elapsed
			if(game->time == 0){
				game->victory = 0;
				break;
			}
			// if user changed dir
			if(game->next_dir != STOP){
				// try to change dir if no wall in that dir
				new_player_y += game->next_dir == UP ? -1 : 0;
				new_player_y += game->next_dir == DOWN ? 1 : 0;
				new_player_x += game->next_dir == RIGHT ? 1 : 0;
				new_player_x += game->next_dir == LEFT ? -1 : 0;
				if(grid[new_player_y][new_player_x] != HOR_WALL && grid[new_player_y][new_player_x] != VER_WALL && grid[new_player_y][new_player_x] != GATE) {
					game->dir = game->next_dir;
					game->next_dir = STOP;
				}
			}
		}
		else {
			game->dir = STOP;
			break;
		}
		delay_ms(80, TIMER_0);
	}
}

char update_game_time(game_t* game){
	if(!game->pause){
		if(game->time > 0){
			game->time -= 1;
		}
		update_time(game);
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

static void input_handler(game_t* game){
	if(joystick_flag & FLAG_JOYSTICK_UP) {
		joystick_flag &= ~FLAG_JOYSTICK_UP;
		game->next_dir = UP;
	}
	if(joystick_flag & FLAG_JOYSTICK_DOWN) {
		joystick_flag &= ~FLAG_JOYSTICK_DOWN;
		game->next_dir = DOWN;
	}
	if(joystick_flag & FLAG_JOYSTICK_LEFT) {
		joystick_flag &= ~FLAG_JOYSTICK_LEFT;
		game->next_dir = LEFT;
	}
	if(joystick_flag & FLAG_JOYSTICK_RIGHT) {
		joystick_flag &= ~FLAG_JOYSTICK_RIGHT;
		game->next_dir = RIGHT;
	}
	// update dir
	if(game->dir == STOP){
		game->dir = game->next_dir;
		game->next_dir = STOP;
	}
}

void pause_handler(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	game->pause = 1;
	render_pause(1);
	while(!(btn_flag & FLAG_BUTTON_0)){ __ASM("wfi"); };
	btn_flag &= ~FLAG_BUTTON_0;
	game->pause = 0;
	render_pause(0);
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
	
	uint32_t game_timer_1s = 1000;
	struct timer_configuration tc = {
		.timer_n = TIMER_1,
		.prescale = 0,
		.mr0 = TIM_MS_TO_TICKS_SIMPLE(game_timer_1s),
		.configuration_mr0 = TIMER_INTERRUPT_MR | TIMER_RESET_MR,
	};
	init_timer(&tc);
	
	// random power pills seed init
	srand(Sys_GetTick());
	game->pp_spawn_counter = (rand() % (INITIAL_TIME / (INITIAL_POWER_PILLS - 1)));
	
	add_player(grid, game);
	render_player(game->player_x, game->player_y);
	
	enable_timer(tc.timer_n, PRIO_3);
		
	while(1){
		__ASM("wfi");
		// Repeat until all flags cleared and game not ended
		while((joystick_flag || game->dir != STOP || btn_flag & FLAG_BUTTON_0) && game->victory == -1){
			if(joystick_flag){
				input_handler(game);
			}
			if(game->dir != STOP){
				move(grid, game);
			}
			if(btn_flag & FLAG_BUTTON_0){
				btn_flag &= ~FLAG_BUTTON_0;
				pause_handler(grid, game);
			}
		}
		if(game->victory != -1){ break; }
	}
	disable_timer(1);
	return game->victory;
}

void win(){
	GUI_Text(100, 100, (uint8_t*) "Victory!", Yellow, Black);
	GUI_Text(80, 120, (uint8_t*) "KEY1 to restart!", Yellow, Black);
	while(!(btn_flag & FLAG_BUTTON_1)){ __ASM("wfi"); };
	btn_flag &= ~FLAG_BUTTON_1;
}

void loose(){
	GUI_Text(100, 100, (uint8_t*) "Game Over!", Yellow, Black);
	GUI_Text(80, 120, (uint8_t*) "KEY1 to restart!", Yellow, Black);
	while(!(btn_flag & FLAG_BUTTON_1)){ __ASM("wfi"); };
	btn_flag &= ~FLAG_BUTTON_1;
}