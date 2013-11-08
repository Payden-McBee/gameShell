<<<<<<< HEAD
<<<<<<< HEAD
# Lab 5 Game Shell

This code provides functions that may prove useful in implementing the Lab 5 game

## Basic Idea

The player is represented by an unsigned char containing a position on the board.  Relies on an LCD library - included as `LCD/LCD.h`.



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

- `void BOOM();`
	-prints BOOM! on the top and bottom lines of the LCD and waits a set time, or until a button is pushed
- `void displayYouWon();`
	-prints You Won! to the LCD screen, you on top and won on bottom
- `unsigned char gameOver();`
	-displays Game on the top LCD line and Over! on the bottom LCD line
- `void setMines(unsigned char firstMine, unsigned char secondMine);`
	-draws an X on the designated locations of the mines 

These are the other funcitons, but are declared in main

- `void generateAndSetMines();`
       -randomly creates locations for two mines
       -if the mines are stacked or diagonal, it will keep generating random locations 
- `void init_timer();`
	-stop timer
	-clear TAR
	-configure for SMCLK
	-divide timer clock by 8
	-clear interrupt flag
	-set count mode to continuous
	-enable interrupt
- `void init_buttons();`
	-set buttons to input
         -enable the interrupts
         -configure interrupt to sense falling edges
         -enable internal pull-up/pull-down network
         -configure as pull-up
         -clear flags
-`#pragma vector=TIMER0_A1_VECTOR;`
	-__interrupt void TIMER0_A1_ISR(void);`
	-clears interrupt flag
	-Flag for continuous counting is TAIFG
	-every 0xffff clock cycles, the timer overflows and the interrupt occurs
	-the timer ticks one count every clock cycle
	-sets flag to 1, saying the timer overflowed




-`#pragma vector=PORT1_VECTOR;`
	-__interrupt void Port_1_ISR(void)
	-configures interrupts for buttons
	-P1IES this is what triggers the interrupt, if it is 1, the interrupt will trigger on
	--a falling edge. If it is 0, the interrupt will trigger on a rising edge. P1 Edge Select.

=======
Lab5_Interrupts
===============

A Simple Game
>>>>>>> 9e0cf5591d1bd2a7e4427fd7c8a2642f78399291
=======
gameShell
=========

gameShell because it won't push to Lab5
>>>>>>> 99ef2e0c530437e40a02e7ce955bd6fafb49484c
