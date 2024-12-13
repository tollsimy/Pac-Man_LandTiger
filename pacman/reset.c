#include "pacman.h"

extern volatile uint8_t btn_flag;

void init_game(game_t* game){
	game->lifes = INITIAL_LIFES;
	game->pills = INITIAL_PILLS;
	game->power_pills = 0;
	game->power_pills_spawn = INITIAL_POWER_PILLS;
	game->score = INITIAL_SCORE;
	game->time = INITIAL_TIME;
	game->player_x = PLAYER_INITIAL_POS_X;
	game->player_y = PLAYER_INITIAL_POS_Y;
	game->dir = STOP;
	game->next_dir = STOP;
	game->victory = 1;
	game->pp_spawn_counter = 0;
	game->pause = 0;
}

void wait_ready(void){
	LCD_Clear(White);
	char ready_str[10] = "READY?";
	GUI_Text(100,150, (uint8_t*)ready_str, Black, White);
	char press_str[25] = "Press KEY1 to start!";
	GUI_Text(50,170, (uint8_t*)press_str, Red, White);
	while(!(btn_flag & FLAG_BUTTON_1));
	btn_flag &= ~FLAG_BUTTON_0;
}
