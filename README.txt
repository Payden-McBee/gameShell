Lab4
====

Device driver for LCD in geek box.

The purpose of this library is to allow the user to interface with the geek box LCD
using an MSP 430. It allows the user to print scrolling messages to the screen, and to
allow user interaction via buttons 1 through 3.

Functions
===

void initSPI(); initializes the Serial Periferal Interface.

void LCDinit(); initializes the LCD screen.

void LCDclear(); Clears the LCD screen.

The above three functions should be implemented in main before using any other functions in this library.

void secondLine(); Moves cursor to the second line.

void firstLine(); Moves cursor to the first line.

void print(char * string); given a pointer to the start of a string, prints the string to the LCD
String must be less than 8 characters.

void scrollString(char * string1, char * string2); given a pointer to the start of two strings, scrolls them
on the first and second line respectively. 

void selectMessage(); allows the user to select a line to scroll accross the bottom of the LCD, along with a 
provided message that scrolls accross the top.