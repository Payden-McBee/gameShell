/*
 * game.h
 * Author: Todd Branchflower
 *
 * This is shell code for the game to be implemented in Lab 4.  It provides basic functions that may prove useful in accomplishing the lab.
 */


#define UP 0x1
#define DOWN 0x2
#define LEFT 0x3
#define RIGHT 0x4

#define TRUE 0x1
#define FALSE 0x0

#define ROW_MASK 0x40

#define NUM_MINES 0x2

#define GO_RIGHT 0x1
#define GO_LEFT 0x2
#define GO_UP 0x3
#define GO_DOWN 0x4
#define NO_DIRECTION 0x0
#define TOP_BOTTOM_DIFFERENCE 0x40

//
// Initializes player to starting position on board.
//
unsigned char initPlayer();

//
// Prints the player marker (*) at the player location passed in.
//
void printPlayer(unsigned char player);

//
// Clears the location passed in.
//
void clearPlayer(unsigned char player);

//
// Given a player's current position and a direction, returns an updated player position.
//
unsigned char movePlayer(unsigned char player, unsigned char direction, unsigned char notOnTopRow, unsigned char notOnBottomRow, unsigned char notOnFarLeft,  unsigned char notOnFarRight);

//
// Returns true if the position passed in is the winning game position.
//
char didPlayerWin(unsigned char player);

unsigned char gameOver();

void setMines();
/*
 * Functions which may prove useful in achieving A-Functionality.
 * 
char didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES]);
void generateMines(unsigned char mines[NUM_MINES]);
void printMines(unsigned char mines[NUM_MINES]);
*/
