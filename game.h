/*
 * game.h
 * Author: Todd Branchflower
 *
 * This is shell code for the game to be implemented in Lab 4.  It provides basic functions that may prove useful in accomplishing the lab.
 */


#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define TRUE 1
#define FALSE 0

#define ROW_MASK 0x40

#define NUM_MINES 2

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
unsigned char movePlayer(unsigned char player, unsigned char direction);

//
// Returns true if the position passed in is the winning game position.
//
char didPlayerWin(unsigned char player);

/*
 * Functions which may prove useful in achieving A-Functionality.
 * 
char didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES]);
void generateMines(unsigned char mines[NUM_MINES]);
void printMines(unsigned char mines[NUM_MINES]);
*/
