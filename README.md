<<<<<<< HEAD
# Lab 5 Game Shell

This code provides functions that may prove useful in implementing the Lab 5 game

## Basic Idea

The player is represented by an unsigned char containing a position on the board.  Relies on an LCD library - included as `LCD/LCD.h`.

`example.c` gives you a program skeleton and pseudocode that may be helpful in achieving required functionality.

## Code

- `unsigned char initPlayer();`
	- returns the address of the first position in the game
	- this can be called to place the player at the starting position
- `void printPlayer(unsigned char player);`
	- prints the player symbol to the passed in location
- `void clearPlayer(unsigned char player);`
	- clears the passed in location by printing a space
- `unsigned char movePlayer(unsigned char player, unsigned char direction);`
	- returns an updated position based on an input position and direction of movement
	- constants for `UP`, `DOWN`, `LEFT`, and `RIGHT` are included in the header
- `char didPlayerWin(unsigned char player);`
	- returns 1 if the player is at the winning position, 0 otherwise

Also provides prototypes of functions that may prove useful in achieving higher levels of functionality.
=======
Lab5_Interrupts
===============

A Simple Game
>>>>>>> 9e0cf5591d1bd2a7e4427fd7c8a2642f78399291
