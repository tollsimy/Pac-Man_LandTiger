#include "pacman.h"

extern uint8_t btn_flag;

void wait_ready(void){
	LCD_Clear(White);
	char ready_str[10] = "READY?";
	GUI_Text(100,150, (uint8_t*)ready_str, Black, White);
	char press_str[25] = "Press KEY1 to start!";
	GUI_Text(50,170, (uint8_t*)press_str, Red, White);
	while(!(btn_flag & FLAG_BUTTON_0));
	btn_flag &= ~FLAG_BUTTON_0;
}
