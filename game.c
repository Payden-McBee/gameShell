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

unsigned char movePlayer(unsigned char player, unsigned char direction, unsigned char notOnTopRow, unsigned char notOnBottomRow, unsigned char notOnFarLeft,  unsigned char notOnFarRight)
{

	switch (direction)
	{
	case GO_RIGHT:
		if(notOnFarRight)
		{
			player++;
		}
		else
		{
			player=0xC0;
		}
		break;
	case GO_LEFT:
		switch (player)
		{
			case 0x80:
				//do nothing
				break;
			case 0xC0:
				player = 0x87;
				break;
			default:
				player--;
				break;
		}
		if(player==0x80)
			{
			//do nothing
			}
		else{
			if(player==0xC0)
			{
				player=0x87;
			}
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

unsigned char gameOver()
{
	unsigned char player=0x80;
	LCDCLR();
	char string1[]="Game      ";
	char string2[]="Over!      ";
	cursorToLineOne();
	writeString(string1);
	cursorToLineTwo();
	writeString(string2);
	longdelay();
	return player;

}
int clearTimer()
{
	return 0;
}

void setMines(unsigned char firstMine, unsigned char secondMine)
{
	writeCommandByte(firstMine);
		writeDataByte('X');
	writeCommandByte(secondMine);
	    writeDataByte('X');
}
