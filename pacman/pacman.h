#ifndef __PACMAN_H__
#define __PACMAN_H__

#include "../common.h"
#include "../include.h"

#define INITIAL_TIME 	60
#define INITIAL_PILLS 240
#define INITIAL_LIFES 3
#define INITIAL_SCORE 0

#define PLAYER_INITIAL_POS_X 13
#define PLAYER_INITIAL_POS_Y 17

#define GRID_RES_X 240
#define GRID_RES_Y 300

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
} cell_t;

volatile typedef struct {
	char pills;
	char lifes;
	char time;
	int score;
	int player_x;
	int player_y;
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
void render_hor_wall(char i, char j);
void render_ver_wall(char i, char j);
void render_pill(char i, char j);
void render_power_pill(char i, char j);
void render_player(char i, char j);
void render_stats(game_t* game);
void render_countdown(uint8_t count);
void render_new_p_pos(int old_player_x, int old_player_y, int player_x, int player_y);
// reset.c
void wait_ready(void);
// ready.c
void draw_game(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
// play.c
uint8_t play_game(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);

#endif