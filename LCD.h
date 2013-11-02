/*
 * lcd.h
 *
 *  Created on: Oct 21, 2013
 *      Author: C15Payden.McBee
 *      Description: Library to interact with the Geek Box LCD.
 *      Documentation: Some of these function names were taken from Capt Branchflower's code.
 */

#ifndef LCD_H_
#define LCD_H_

#define RS_MASK 0x40
#define E 0x80
#define GIVE_COMMAND 0x0 //set Rs to 0 to give a command to LCD
#define SECOND_LINE 0xC0
#define FIRST_SPACE_LCD 0x40
#define NUM_SIGN 0x23



void INITSPI();
void LCDINIT();
void LCDCLR();

void SPISEND(char byteToSend);
void SET_SS_HI();
void SET_SS_LO();
void LCDDELAY1();
void LCDDELAY2();
void LCDWRT8(char byteToSend);
void LCDWRT4(char LCDDATA);
void longdelay();

void rotateString(char string[]);
void cursorToLineTwo();
void cursorToLineOne();
void writeChar(char asciiChar);
void writeString(char string[]);
void scrollString(char string1[], char string2[]);

void writeDataByte(char dataByte);
void writeCommandNibble(char commandNibble);
void writeCommandByte(char commandByte);
void writeDataByte(char dataByte);


#endif /* LCD_H_ */
