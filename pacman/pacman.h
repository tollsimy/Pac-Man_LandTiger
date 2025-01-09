#ifndef __PACMAN_H__
#define __PACMAN_H__

#include "../common.h"
#include "../include.h"

#define INITIAL_TIME 	60
#define INITIAL_PILLS 240
#define INITIAL_LIFES 1
#define INITIAL_SCORE 0
#define INITIAL_POWER_PILLS 6

#define PLAYER_INITIAL_POS_X 13
#define PLAYER_INITIAL_POS_Y 17

#define GRID_RES_X 240
#define GRID_RES_Y 300

#define BORDER_X 8
#define BORDER_Y 25

#define GRID_WIDTH 28
#define GRID_HEIGHT 31

#define CELL_SIZE_X GRID_RES_X/GRID_WIDTH
#define CELL_SIZE_Y GRID_RES_Y/GRID_HEIGHT

volatile typedef enum {
	EMPTY,
	PILL,
	POWER_PILL,
	HOR_WALL,
	VER_WALL,
	PLAYER,
	GATE,
} cell_t;

volatile typedef enum {
	STOP,
	RIGHT,
	LEFT,
	DOWN,
	UP,
} dir_t;

volatile typedef struct {
	char pills;
	char power_pills;
	char power_pills_spawn;
	char lifes;
	char time;
	int score;
	int player_x;
	int player_y;
	dir_t dir;
	dir_t next_dir;
	int8_t victory;
	uint32_t pp_spawn_counter;
	char pause;
} game_t;

volatile typedef enum {
	STATE_RESET,
	STATE_READY,
	STATE_PLAY,
	STATE_WIN,
	STATE_LOOSE
} state_t;

void pacman();
// graphic.c
void render_hor_wall(char x, char y);
void render_ver_wall(char x, char y);
void render_pill(char x, char y);
void render_power_pill(char x, char y);
void render_player(char x, char y);
void render_gate(char x, char y);
void render_stats(game_t* game);
void update_stats(game_t* game);
void update_time(game_t* game);
void render_countdown(uint8_t count);
void clear_countdown();
void render_new_p_pos(int old_player_x, int old_player_y, int player_x, int player_y);
void render_pause(char val);
// reset.c
void init_game(game_t* game);
void wait_ready(void);
// ready.c
void draw_game(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
// play.c
uint8_t play_game(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
char update_game_time(game_t* game);
void spawn_random_pp(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
void win();
void loose();
// pacman_IRQ.c
void pacman_timer_IRQ();

#endif