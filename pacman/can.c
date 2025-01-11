#include "pacman.h"

#define CAN_CTRL 1
#define CAN_ID 0x2

extern CAN_msg CAN_TxMsg;

void CAN_send_stats(game_t* game){	
	CAN_TxMsg.id = CAN_ID;
	
	CAN_TxMsg.data[0] = (game->time & 0xFF);
	CAN_TxMsg.data[1] = (game->lifes & 0xFF);
	CAN_TxMsg.data[2] = (game->score & 0x00FF);
	CAN_TxMsg.data[3] = (game->score & 0xFF00) >> 8; 

	CAN_TxMsg.len = 4;
	CAN_TxMsg.format = 0;
	CAN_TxMsg.type = 0;

	CAN_wrMsg(CAN_CTRL, &CAN_TxMsg);
}
