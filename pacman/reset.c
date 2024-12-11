#include "pacman.h"

extern uint8_t btn_flag;

void wait_ready(void){
	LCD_Clear(White);
	char ready_str[10] = "READY?";
	GUI_Text(150,150, (uint8_t*)ready_str, Black, White);
	while(1){
		if(btn_flag & 0x1){
			break;
		}
	}
}
