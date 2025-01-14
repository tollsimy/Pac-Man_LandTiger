#ifndef __PACMAN_H__
#define __PACMAN_H__

#include "../common.h"
#include "../include.h"
#include "../CAN/CAN.h"
#include "../music/music.h"

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

#define ENEMY_NUM 1

#define INITIAL_ENEMY_X 13
#define INITIAL_ENEMY_Y 15

typedef struct {
	NOTE* melody;
	int length;
} Melody;

volatile typedef enum {
	EMPTY,
	PILL,
	POWER_PILL,
	HOR_WALL,
	VER_WALL,
	PLAYER,
	ENEMY,
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
	uint16_t score;
	int player_x;
	int player_y;
	int enemy_x[ENEMY_NUM];
	int enemy_y[ENEMY_NUM];
	dir_t pdir;
	dir_t edir[ENEMY_NUM];
	uint8_t enemy_fright;
	dir_t next_pdir;
	int8_t victory;
	uint32_t pp_spawn_counter;
	char pause;
	char started;
	Melody melody;
} game_t;

volatile typedef enum {
	STATE_RESET,
	STATE_READY,
	STATE_PLAY,
	STATE_WIN,
	STATE_LOOSE
} state_t;

// music

static NOTE MELODY_PACMAN[] = {
	{NOTE_B4, time_16th},
	{NOTE_B5, time_16th},
	{NOTE_FS5, time_16th},
	{NOTE_DS5, time_16th},
	{NOTE_B5, time_32nd},
	{NOTE_FS5, time_16th_dotted},
	{NOTE_DS5, time_8th},
	{NOTE_C5, time_16th},
	{NOTE_C6, time_16th},
	{NOTE_G6, time_16th},
	{NOTE_E6, time_16th},
	{NOTE_C6, time_32nd},
	{NOTE_G6, time_16th_dotted},
	{NOTE_E6, time_8th},
	
	{NOTE_B4, time_16th},
	{NOTE_B5, time_16th},
	{NOTE_FS5, time_16th},
	{NOTE_DS5, time_16th},
	{NOTE_B5, time_32nd},
	{NOTE_FS5, time_16th_dotted},
	{NOTE_DS5, time_8th},
	{NOTE_DS5, time_32nd},
	{NOTE_E5, time_32nd},
	{NOTE_F5, time_32nd},
	{NOTE_F5, time_32nd},
	{NOTE_FS5, time_32nd},
	{NOTE_G5, time_32nd},
	{NOTE_G5, time_32nd},
	{NOTE_GS5, time_32nd},
	{NOTE_A5, time_16th},
	{NOTE_B5, time_8th},
};

static NOTE MELODY_P_PILL[] = {
	{NOTE_A5, time_16th},
	{NOTE_F5, time_16th},
};

//

void pacman();
// graphic.c
void render_hor_wall(char x, char y);
void render_ver_wall(char x, char y);
void render_pill(char x, char y);
void render_power_pill(char x, char y);
void render_player(char x, char y, int angle);
void render_enemy(char x, char y, int angle, uint8_t fright);
void render_gate(char x, char y);
void render_stats(game_t* game);
void update_stats_CAN(CAN_msg CAN_RxMsg, game_t* game);
void update_time_CAN(CAN_msg CAN_RxMsg, game_t* game);
void render_countdown(uint8_t count);
void clear_countdown();
void render_new_e_pos(int old_player_x, int old_player_y, int player_x, int player_y, cell_t prev_cell, int angle, uint8_t fright);
void render_new_p_pos(int old_player_x, int old_player_y, int player_x, int player_y, int angle);
void render_pause(char val);
// reset.c
void init_game(game_t* game);
void wait_ready(game_t* game);
// ready.c
void draw_game(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
// play.c
uint8_t play_game(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
void move(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
void move_enemies(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
uint8_t check_collision(game_t* game);
void respawn_pacman(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
char update_stats(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
char update_game_time(game_t* game);
void spawn_random_pp(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game);
void pause_handler(game_t* game, int pause_val);
void win();
void loose();
// pacman_IRQ.c
void pacman_timer_IRQ();
// can.c
void CAN_send_stats(game_t* game);
// pacman_music.c
void play_melody_note(NOTE melody[], int lenght);
void enable_melody();

#endif