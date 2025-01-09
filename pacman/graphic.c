#include "pacman.h"

#define GET_X(grid_x) ((grid_x) * (CELL_SIZE_X) + BORDER_X)
#define GET_Y(grid_y) ((grid_y) * (CELL_SIZE_Y) + BORDER_Y)

#define GET_X_CENTER(grid_x) (GET_X(grid_x) + (CELL_SIZE_X) / 2)
#define GET_Y_CENTER(grid_y) (GET_Y(grid_y) + (CELL_SIZE_Y) / 2)

static void render_rect(int x, int y, int width, int height, uint16_t color){
	for(int i=x;i<x+width;i++){
		for(int j=y;j<y+height;j++){
			LCD_SetPoint(i,j,color);
		}
	}
};

void render_hor_wall(char x, char y){
	render_rect(GET_X(x), GET_Y(y), CELL_SIZE_X, CELL_SIZE_Y, Blue);
};
void render_ver_wall(char x, char y){
	render_rect(GET_X(x), GET_Y(y), CELL_SIZE_X, CELL_SIZE_Y, Blue);
};
void render_pill(char x, char y){
	render_rect(GET_X(x), GET_Y(y), CELL_SIZE_X, CELL_SIZE_Y, Black);
	LCD_draw_circle(GET_X_CENTER(x),GET_Y_CENTER(y),2,Yellow);
};
void render_power_pill(char x, char y){
	render_rect(GET_X(x), GET_Y(y), CELL_SIZE_X, CELL_SIZE_Y, Black);
	LCD_draw_circle(GET_X_CENTER(x),GET_Y_CENTER(y),3,Red);
};
void render_player(char x, char y){
	render_rect(GET_X(x), GET_Y(y), CELL_SIZE_X, CELL_SIZE_Y, Black);
	LCD_draw_circle(GET_X_CENTER(x),GET_Y_CENTER(y),4,Green);
};
void render_new_p_pos(int old_player_x, int old_player_y, int player_x, int player_y){
	render_rect(GET_X(old_player_x), GET_Y(old_player_y), CELL_SIZE_X, CELL_SIZE_Y, Black);
	render_player(player_x, player_y);
};
void render_gate(char x, char y){
	render_rect(GET_X(x), GET_Y(y), CELL_SIZE_X, CELL_SIZE_Y, Black);
	render_rect(GET_X(x), GET_Y(y), CELL_SIZE_X, CELL_SIZE_Y, Yellow);
};

void render_stats(game_t* game){
	char str[40];
	sprintf(str, "Time: %02d, Score: %04d", game->time, game->score);
	GUI_Text(30, 5, (uint8_t*)str, White, Black);
	
	sprintf(str, "Lifes: %01d", game->lifes);
	GUI_Text(10, 305, (uint8_t*)str, Red, Black);
};

void update_stats(game_t* game){
	char str[10];
	sprintf(str, "%04d", game->score);
	GUI_Text(167, 5, (uint8_t*)str, White, Black);
	
	sprintf(str, "%01d", game->lifes);
	GUI_Text(65, 305, (uint8_t*)str, Red, Black);
};

void update_time(game_t* game){
	uint16_t color = White;
	char str[10];
	sprintf(str, "%02d", game->time);
	if(game->time < 5){
		color = Red;
	}
	GUI_Text(79, 5, (uint8_t*)str, color, Black);
};

void render_countdown(uint8_t count){
	render_rect(96, 142, 48, 28, Red);
	char count_str[20];
	sprintf(count_str, "%dS", count);
	GUI_Text(105,150,(uint8_t*)count_str, Yellow, Red);
};

void clear_countdown(){
	render_rect(96, 142, 48, 28, Black);
};

void render_pause(char val){
	if(val){
		GUI_Text(180,305,(uint8_t*)"Pause", Yellow, Black);
	} else {
		render_rect(180,305,40,15,Black);
	}
}
