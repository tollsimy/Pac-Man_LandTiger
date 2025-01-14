#include "pacman.h"

extern volatile uint8_t btn_flag;

void init_game(game_t* game){
	game->lifes = INITIAL_LIFES;
	game->pills = INITIAL_PILLS;
	game->power_pills = 0;
	game->power_pills_spawn = INITIAL_POWER_PILLS;
	game->score = INITIAL_SCORE;
	game->time = INITIAL_TIME;
	game->player->player_x = PLAYER_INITIAL_POS_X;
	game->player->player_y = PLAYER_INITIAL_POS_Y;
	for(int i=0; i<ENEMY_NUM; i++){
		game->enemy->enemy_x[i]= INITIAL_ENEMY_X;
		game->enemy->enemy_y[i]= INITIAL_ENEMY_Y;
		game->enemy->edir[i] = STOP;
	}
	game->enemy->enemy_fright = 0;
	game->enemy->enemy_delay = INITIAL_ENEMY_DELAY;
	game->player->pdir = STOP;
	game->player->next_pdir = STOP;
	game->victory = -1;
	game->pp_spawn_counter = 0;
	game->pause = 0;
	game->started = 0;
}

void wait_ready(game_t* game){
	LCD_Clear(White);
	char ready_str[10] = "READY?";
	GUI_Text(100,150, (uint8_t*)ready_str, Black, White);
	game->melody.melody = MELODY_PACMAN;
	game->melody.length = sizeof(MELODY_PACMAN)/sizeof(MELODY_PACMAN[0]);
	enable_melody();
	char press_str[25] = "Press KEY1 to start!";
	GUI_Text(50,170, (uint8_t*)press_str, Red, White);
	btn_flag &= ~FLAG_BUTTON_1;
	while(!(btn_flag & FLAG_BUTTON_1)){ __ASM("wfi"); };
	btn_flag &= ~FLAG_BUTTON_1;
}
