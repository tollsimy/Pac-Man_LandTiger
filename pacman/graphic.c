#include "pacman.h"

#define GET_X(grid_x) ((grid_x) * (CELL_SIZE_X) + BORDER_X)
#define GET_Y(grid_y) ((grid_y) * (CELL_SIZE_Y) + BORDER_Y)

#define GET_X_CENTER(grid_x) (GET_X(grid_x) + (CELL_SIZE_X) / 2)
#define GET_Y_CENTER(grid_y) (GET_Y(grid_y) + (CELL_SIZE_Y) / 2)

const uint8_t pacman_1_sprite[8][8] = {
  {0, 0, 0, 1, 1, 1, 1, 0}, // Row 0
  {0, 0, 1, 1, 1, 1, 1, 1}, // Row 1
  {0, 1, 1, 1, 1, 1, 1, 0}, // Row 2
  {0, 1, 1, 1, 1, 1, 0, 0}, // Row 3
  {0, 1, 1, 1, 1, 1, 0, 0}, // Row 4
  {0, 1, 1, 1, 1, 1, 1, 0}, // Row 5
  {0, 0, 1, 1, 1, 1, 1, 1}, // Row 6
  {0, 0, 0, 1, 1, 1, 1, 0}  // Row 7
};
const uint8_t pacman_2_sprite[8][8] = {
  {0, 0, 0, 1, 1, 1, 1, 1}, // Row 0
  {0, 0, 1, 1, 1, 1, 1, 0}, // Row 1
  {0, 1, 1, 1, 1, 1, 0, 0}, // Row 2
  {0, 1, 1, 1, 1, 0, 0, 0}, // Row 3
  {0, 1, 1, 1, 1, 0, 0, 0}, // Row 4
  {0, 1, 1, 1, 1, 1, 0, 0}, // Row 5
  {0, 0, 1, 1, 1, 1, 1, 0}, // Row 6
  {0, 0, 0, 1, 1, 1, 1, 1}  // Row 7
};

const uint8_t l_heart_sprite[8][8] = {
  {0, 0, 1, 1, 1, 1, 0, 0}, // Row 0
  {0, 1, 1, 1, 1, 1, 1, 0}, // Row 1
  {1, 1, 1, 1, 1, 1, 1, 1}, // Row 2
  {0, 1, 1, 1, 1, 1, 1, 1}, // Row 3
  {0, 0, 1, 1, 1, 1, 1, 1}, // Row 4
  {0, 0, 0, 0, 1, 1, 1, 1}, // Row 5
  {0, 0, 0, 0, 0, 0, 1, 1}, // Row 6
  {0, 0, 0, 0, 0, 0, 0, 1}  // Row 7
};

const uint8_t r_heart_sprite[8][8] = {
  {0, 0, 1, 1, 1, 1, 0, 0}, // Row 0
  {0, 1, 1, 1, 1, 1, 1, 0}, // Row 1
  {1, 1, 1, 1, 1, 1, 1, 1}, // Row 2
  {1, 1, 1, 1, 1, 1, 1, 0}, // Row 3
  {1, 1, 1, 1, 1, 1, 0, 0}, // Row 4
  {1, 1, 1, 1, 0, 0, 0, 0}, // Row 5
  {1, 1, 0, 0, 0, 0, 0, 0}, // Row 6
  {1, 0, 0, 0, 0, 0, 0, 0}  // Row 7
};

const uint8_t pill_sprite[8][8] = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 0, 0},
	{0, 0, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
};

const uint8_t p_pill_sprite[8][8] = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
};

static void render_sprite(int x, int y, const uint8_t matrix[8][8], uint16_t color) {
	int height = 8;
	int width = 8;
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      if (matrix[row][col]) {
        LCD_SetPoint(x + col, y + row, color);
      }
    }
  }
}

static void rotate_sprite(const uint8_t original[8][8], uint8_t rotated[8][8], int angle) {
	int height = 8;
	int width = 8;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      rotated[i][j] = 0;
    }
  }
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      if (original[row][col]) {
        switch (angle) {
          case 90: // 90 clockwise
            rotated[col][height - 1 - row] = 1;
            break;
          case 180: // 180
            rotated[height - 1 - row][width - 1 - col] = 1;
            break;
          case 270: // 90 counterclockwise
            rotated[width - 1 - col][row] = 1;
            break;
          default: // 0 (no rotation)
            rotated[row][col] = 1;
            break;
        }
      }
    }
  }
}

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
	render_sprite(GET_X(x), GET_Y(y), pill_sprite, Yellow);
};
void render_power_pill(char x, char y){
	render_rect(GET_X(x), GET_Y(y), CELL_SIZE_X, CELL_SIZE_Y, Black);
	render_sprite(GET_X(x), GET_Y(y), p_pill_sprite, Red);
};

void render_player(char x, char y, int angle) {
	static uint8_t toggle_img = 1;
	uint8_t rotated[8][8];
	if(toggle_img){
		rotate_sprite(pacman_1_sprite, rotated, angle);
		toggle_img = !toggle_img;
	} else {
		rotate_sprite(pacman_2_sprite, rotated, angle);
		toggle_img = !toggle_img;
	}
	render_sprite(GET_X(x), GET_Y(y), rotated, Yellow);
};
void render_new_p_pos(int old_player_x, int old_player_y, int player_x, int player_y, int angle){
	render_rect(GET_X(old_player_x), GET_Y(old_player_y), CELL_SIZE_X, CELL_SIZE_Y, Black);
	render_player(player_x, player_y, angle);
};

void render_gate(char x, char y){
	render_rect(GET_X(x), GET_Y(y), CELL_SIZE_X, CELL_SIZE_Y, Black);
	render_rect(GET_X(x), GET_Y(y), CELL_SIZE_X, CELL_SIZE_Y, Yellow);
};

void render_enemy(char x, char y, int angle) {
	static uint8_t toggle_img = 1;
	uint8_t rotated[8][8];
	if(toggle_img){
		rotate_sprite(pacman_1_sprite, rotated, angle);
		toggle_img = !toggle_img;
	} else {
		rotate_sprite(pacman_2_sprite, rotated, angle);
		toggle_img = !toggle_img;
	}
	render_sprite(GET_X(x), GET_Y(y), rotated, Yellow);
};

void render_stats(game_t* game){
	char str[40];
	sprintf(str, "Time: %02d, Score: %04d", game->time, game->score);
	GUI_Text(30, 5, (uint8_t*)str, White, Black);
	
	for(int i=0; i<(game->lifes); i++){
		render_sprite(10 + (16*i) , 305, l_heart_sprite, Red);
		render_sprite(18 + (16*i), 305, r_heart_sprite, Red);
	}
};

void update_stats_CAN(CAN_msg CAN_RxMsg, game_t* game){
	char str[10];
	static int prev_lifes;
	static int prev_score;
	
	if(prev_lifes != game->lifes){
	render_rect(10, 305, 16*5, 8, Black);
	
#ifdef SIMULATOR
		for(int i=0; i<(game->lifes); i++){
			render_sprite(10 + (16*i), 305, l_heart_sprite, Red);
			render_sprite(18 + (16*i), 305, r_heart_sprite, Red);
		}
#else
		for(int i=0; i<(CAN_RxMsg.data[1]); i++){
			render_sprite(10 + (16*i), 305, l_heart_sprite, Red);
			render_sprite(18 + (16*i), 305, r_heart_sprite, Red);
		}
#endif
		prev_lifes = game->lifes;
	}
	
	if(prev_score != game->score){
#ifdef SIMULATOR
		sprintf(str, "%04d", game->score);
		GUI_Text(167, 5, (uint8_t*)str, White, Black);
#else
		uint16_t score = CAN_RxMsg.data[2] +  (CAN_RxMsg.data[3] << 8);
		sprintf(str, "%04d", score);
		GUI_Text(167, 5, (uint8_t*)str, White, Black);
#endif
		prev_score = game->score;
	}
}

void update_time_CAN(CAN_msg CAN_RxMsg, game_t* game){
	char str[10];
	uint16_t color = White;
	if(game->time < 5){
		color = Red;
	}
	
#ifdef SIMULATOR
	sprintf(str, "%02d", game->time);
	GUI_Text(79, 5, (uint8_t*)str, color, Black);
#else
	sprintf(str, "%02d", CAN_RxMsg.data[0]);
	GUI_Text(79, 5, (uint8_t*)str, color, Black);
#endif
}

void render_countdown(uint8_t count){
	render_rect(96, 142, 48, 28, Red);
	char count_str[20];
	sprintf(count_str, "%dS", count);
	GUI_Text(110,150,(uint8_t*)count_str, Yellow, Red);
};

void clear_countdown(){
	render_rect(96, 142, 48, 28, Black);
};

void render_new_e_pos(int old_enemy_x, int old_enemy_y, int enemy_x, int enemy_y, cell_t prev_cell, int angle){
	int y = GET_Y(old_enemy_y);
	int x = GET_X(old_enemy_x);
	render_enemy(enemy_y, enemy_x, angle);
	switch (prev_cell){
		case EMPTY:
			render_rect(x, y, CELL_SIZE_X, CELL_SIZE_Y, Black);
			break;
		case PILL:
			render_pill(old_enemy_y, old_enemy_x);
			break;
		case POWER_PILL:
			render_power_pill(old_enemy_y, old_enemy_x);
			break;
		default:
			break;
	}
};

void render_pause(char val){
	if(val){
		GUI_Text(180,305,(uint8_t*)"Pause", Yellow, Black);
	} else {
		render_rect(180,305,40,15,Black);
	}
}
