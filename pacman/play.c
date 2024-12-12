#include "pacman.h"

extern uint8_t joystick_flag;
extern uint8_t btn_flag;

static void add_player(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	grid[game->player_y][game->player_x] = PLAYER;
}

static void move(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game, uint8_t dir){	
	char old_joystick_flag = joystick_flag;
	while(old_joystick_flag == joystick_flag){
		int old_player_x = game->player_x;
		int old_player_y = game->player_y;
		
		int new_player_y = game->player_y;
		int new_player_x = game->player_x;
		new_player_y += dir == FLAG_JOYSTICK_UP ? -1 : 0;
		new_player_y += dir == FLAG_JOYSTICK_DOWN ? 1 : 0;
		new_player_x += dir == FLAG_JOYSTICK_RIGHT ? 1 : 0;
		new_player_x += dir == FLAG_JOYSTICK_LEFT ? -1 : 0;
		
		if(grid[new_player_y][new_player_x] != HOR_WALL && grid[new_player_y][new_player_x] != VER_WALL) {
			game->player_x = new_player_x;
			game->player_y = new_player_y;
			if(grid[game->player_y][game->player_x] == PILL){
				game->pills -=1;
				game->score += 10;
			} else if(grid[game->player_y][game->player_x] == POWER_PILL) {
				game->pills -=1;
				game->score += 50;
			}
			grid[game->player_y][game->player_x] = PLAYER;
			grid[old_player_y][old_player_x] = EMPTY;
			render_new_p_pos(old_player_x, old_player_y, game->player_x, game->player_y);
			render_stats(game);
		}
		else {
			break;
		}
		int time = Sys_GetTick();
		while(Sys_GetTick() - time < 100){}
	}
}

uint8_t play_game(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	// Countdown 3s
	uint8_t count = 4;
	uint32_t time = Sys_GetTick();
	while(count){
		if(time - Sys_GetTick() > 1000){
			render_countdown(count-1);
			time += 1000;
			count--;
		}
	}
	add_player(grid, game);
	draw_game(grid, game);
	
	while(1){
		if(joystick_flag & FLAG_JOYSTICK_UP) {
			joystick_flag &= ~FLAG_JOYSTICK_UP;
			move(grid, game, FLAG_JOYSTICK_UP);
		}
		if(joystick_flag & FLAG_JOYSTICK_DOWN) {
			joystick_flag &= ~FLAG_JOYSTICK_DOWN;
			move(grid, game, FLAG_JOYSTICK_DOWN);
		}
		if(joystick_flag & FLAG_JOYSTICK_LEFT) {
			joystick_flag &= ~FLAG_JOYSTICK_LEFT;
			move(grid, game, FLAG_JOYSTICK_LEFT);
		}
		if(joystick_flag & FLAG_JOYSTICK_RIGHT) {
			joystick_flag &= ~FLAG_JOYSTICK_RIGHT;
			move(grid, game, FLAG_JOYSTICK_RIGHT);
		}
		if(joystick_flag & FLAG_JOYSTICK_SELECT) {
			joystick_flag &= ~FLAG_JOYSTICK_SELECT;
		}
	}

	return 1;
}
