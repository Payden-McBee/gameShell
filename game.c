#include <msp430g2553.h>
#include "game.h"
#include "LCD.h"

unsigned char initPlayer()
{

	return 0x80;
}

void printPlayer(unsigned char player)
{
	writeCommandByte(player);
	writeDataByte('*');
}

void clearPlayer(unsigned char player)
{
	writeCommandByte(player);
	writeDataByte(' ');
}

unsigned char movePlayer(unsigned char player, unsigned char direction, unsigned char notOnTopRow, unsigned char notOnBottomRow)
{

	switch (direction)
	{
	case GO_RIGHT:
		//if(notOnFarRight)
		{
			player++;
		}
		break;
	case GO_LEFT:
	//	if(notOnFarLeft)
			{
			player--;
			}
		break;
	case GO_UP:
		if(notOnTopRow)
			{
			player=player-TOP_BOTTOM_DIFFERENCE;
			}
		break;
	case GO_DOWN:
		if(notOnBottomRow)
			{
			player=player+TOP_BOTTOM_DIFFERENCE;
			}
		break;
		//
		// update player position based on direction of movement
		//
	}

	return player;
}

char didPlayerWin(unsigned char player)
{
	return player == 0xC7;
}
