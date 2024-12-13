#include "pacman.h"

static int get_x(char x){
	return x*(CELL_SIZE_X);
};

static int get_y(char y){
	return y*(CELL_SIZE_Y);
};

static void render_square(int x, int y, int width, int height, uint16_t color){
	for(int i=x;i<x+width;i++){
		for(int j=y;j<y+height;j++){
			LCD_SetPoint(i,j,color);
		}
	}
};

void render_hor_wall(char i, char j){
	int y = get_y(i);
	int x = get_x(j);
	/*
	int width = CELL_SIZE_X;
	int height = CELL_SIZE_Y;
	
	for(int i=x;i<x+width;i++){
		for(int j=y;j<y+height;j++){
			LCD_SetPoint(i,j,Black);
		}
	}
	LCD_DrawLine(x,y,x+width,y,Blue);
	LCD_DrawLine(x,y+height,x+width,y+height,Blue);
	*/
	render_square(x,y, CELL_SIZE_X, CELL_SIZE_Y, Blue);
};
void render_ver_wall(char i, char j){
	int y = get_y(i);
	int x = get_x(j);
	/*
	int width = CELL_SIZE_X;
	int height = CELL_SIZE_Y;
	
	for(int i=x;i<x+width;i++){
		for(int j=y;j<y+height;j++){
			LCD_SetPoint(i,j,Black);
		}
	}
	LCD_DrawLine(x,y,x,y+height,Blue);
	LCD_DrawLine(x+width,y,x+width,y+height,Blue);
	*/
	render_square(x,y, CELL_SIZE_X, CELL_SIZE_Y, Blue);
};
void render_pill(char i, char j){
	int y = get_y(i) + CELL_SIZE_Y / 2;
	int x = get_x(j) + CELL_SIZE_X / 2;
	LCD_draw_circle(x,y,2,Yellow);
};
void render_power_pill(char i, char j){
	int y = get_y(i) + CELL_SIZE_Y / 2;
	int x = get_x(j) + CELL_SIZE_X / 2;
	LCD_draw_circle(x,y,3,Red);
};
void render_player(char i, char j){
	int y = get_y(i) + CELL_SIZE_Y / 2;
	int x = get_x(j) + CELL_SIZE_X / 2;
	LCD_draw_circle(x,y,4,Green);
};

void render_stats(game_t* game){
	char time_str[30];
	sprintf(time_str, "Time: %d, Lifes: %d, Score:%d", game->time, game->lifes, game->score);
	GUI_Text(0, 300, (uint8_t*)time_str, White, Black);
};

void render_countdown(uint8_t count){
	render_square(70,120, 80, 50, Red);
	char count_str[20];
	sprintf(count_str, "%dS", count);
	GUI_Text(105,135,(uint8_t*)count_str, Yellow, Red);
};

void render_new_p_pos(int old_player_x, int old_player_y, int player_x, int player_y){
	int y = get_y(old_player_y);
	int x = get_x(old_player_x);
	render_square(x, y, CELL_SIZE_X, CELL_SIZE_Y, Black);
	render_player(player_y, player_x);
};

void render_pause(game_t* game){
	render_square(70,120, 80, 50, White);
	GUI_Text(105,135,(uint8_t*)"Pause", Yellow, White);
}
