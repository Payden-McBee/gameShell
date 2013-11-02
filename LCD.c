/*
 * lcd_functions.c
 *
 *  Created on: Oct 21, 2013
 *      Author: C15Payden.McBee
 */
#include <msp430.h>
#include "LCD.h"


unsigned char LCDCON;
unsigned char LCDSEND;

/*---------------------------------------------------
;Function Name: INITSPI
;Author: C2C Payden McBee, USAF
;Function: initializes the SPI
;Inputs:none
;Outputs: none
;Registers destroyed: none
;---------------------------------------------------*/
void INITSPI(){
	  //your SPI initialization code goes here
		// |= bis.s
		// &= ~  bis.c
						UCB0CTL1|=UCSWRST;

					    UCB0CTL0|=UCCKPH; 	//The LCD driver chip expects data to be read on the first clock edge and changed on the second.

						UCB0CTL1&= ~UCCKPL; 	//The LCD driver chip expects the clock should be low when not trasnmitting.

	       			    UCB0CTL0|=UCMSB|UCMST|UCSYNC;     // don't forget UCSYNC!

	                    UCB0CTL1|=UCSSEL1;             // select a clock to use!

	                    UCB0STAT|=UCLISTEN;   //enables internal loopback

	                    P1SEL|=BIT5;   //make UCA0CLK available on P1.4
	                    P1SEL2|=BIT5;

	                    P1SEL|=BIT7;          // make UCA0SSIMO available on P1.2
	                    P1SEL2|=BIT7;

	       			    P1SEL|=BIT6;          //make UCA0SSOMI available on P1.1
	       				P1SEL2|=BIT6;

	      				UCB0CTL1 &= ~UCSWRST;     //enable subsystem

	      			    P1DIR|= BIT0;

}

/*---------------------------------------------------
; Function Name: LCDINIT
; Author: Capt Todd Branchflower, USAF
; Function: Initializes the LCD on the Geek Box
; Inputs: none
; Outputs: none
; Registers destroyed: none
; Functions used: LCDWRT4, LCDWRT8, LCDDELAY1, LCDDELAY2
;---------------------------------------------------*/
void LCDINIT(){
	  	  	  	  	  SET_SS_HI();

	                  writeCommandNibble(0x03);	 // function set

	                  writeCommandNibble(0x03);	//function set

	                  writeCommandNibble(0x03);	 // function set

	                  writeCommandNibble(0x02);	 //set 4-bit interface

	                  writeCommandByte(0x28);	 // 2 lines, 5x7

	                  writeCommandByte(0x0C);	 // display on, cursor, blink off

	                  writeCommandByte(0x01);	 //clear, cursor home

	                  writeCommandByte(0x06);	 //cursor increment, shift off

	                  writeCommandByte(0x01);	 //clear, cursor home

	                  writeCommandByte(0x02);	//cursor home

	                  SPISEND(0);
	                  LCDDELAY1();
}

/*---------------------------------------------------
;Function Name: SET_SS_HI
;Author: C2C Payden McBee, USAF
;Function:  Sets your slave select to high (disabled)
;Inputs:none
;Outputs: none
;Registers destroyed: none
;Functions used:none
;---------------------------------------------------*/
void SET_SS_HI(){

              //your set SS high code goes here

				P1OUT|=BIT0;

}

/*Function Name: SET_SS_LO
;Author: C2C Payden McBee, USAF
;Function:  Sets your slave select to low (enabled)
;Inputs:none
;Outputs: none
;Registers destroyed: none
;Functions used:none
*/
void SET_SS_LO(){

                  //your set SS low code goes here
				P1OUT&=~BIT0;

}


/*---------------------------------------------------
;Function Name: LCDDELAY1
;Author: C2C Payden McBee, USAF
;Function: Implements a 40.5 microsecond delay
;Inputs:delay1, a constant number to iterate the loop
;Outputs: none
;Registers destroyed: none
;Functions used:none
;---------------------------------------------------*/
void LCDDELAY1(){

                  //your 40.5 microsecond delay  //43 cycles
                  _delay_cycles(43);
}


/*;---------------------------------------------------
;Function Name: LCDDELAY1
;Author: C2C Payden McBee, USAF
;Function: Implements a 1.65 millisecond delay
;Inputs:delay1, a constant number to iterate the loop
;Outputs: none
;Registers destroyed: none
;Functions used:none
;---------------------------------------------------*/
void LCDDELAY2(){

                 // your 1.65 millisecond delay code goes here  //1741 cycles
				_delay_cycles(1741);
}

/*---------------------------------------------------
; Function Name: LCDCLR
; Author: Capt Todd Branchflower, USAF
; Function: Clears LCD, sets cursor to home
; Inputs: none
; Outputs: none
; Registers destroyed: none
; Functions used: LCDWRT8, LCDDELAY1, LCDDELAY2
;---------------------------------------------------*/
void LCDCLR(){
		writeCommandByte(1);
}

/*---------------------------------------------------
; Function Name: LCDWRT8
; Author: Capt Todd Branchflower, USAF
; Function: Send full byte to LCD
; Inputs: LCDSEND
; Outputs: none
; Registers destroyed: none
; Functions used: LCDWRT4
;---------------------------------------------------*/
void LCDWRT8(char byteToSend)
{
    	unsigned char sendByte = byteToSend;

    	sendByte &= 0xF0;

    	sendByte = sendByte >> 4;               // rotate to the right 4 times

    	LCDWRT4(sendByte);

    	sendByte = byteToSend;

    	sendByte &= 0x0F;

    	LCDWRT4(sendByte);
}

/*---------------------------------------------------
; Function Name: LCDWRT4
; Author: C2C Payden McBee
; Function: Send 4 bits of data to LCD via SPI.
; sets upper four bits to match LCDCON.
; Inputs: LCDCON, LCDDATA
; Outputs: none
; Registers destroyed: none
; Functions used: LCDDELAY1
; Documentation: The code was created by
;  Capt Branchflower in assembly, I converted it to C.
;---------------------------------------------------*/
void LCDWRT4(char LCDDATA){

                  unsigned char sendByte;
                  sendByte=LCDDATA;		    //load data to send
                  sendByte&=0x0f;			//ensure upper half of byte is clear
                  sendByte|=LCDCON;         //set LCD control nibble
				  sendByte&=~E;       //set E low
                  SPISEND(sendByte);
                  LCDDELAY1();
                  sendByte|=E;	                 //set E high
                  SPISEND(sendByte);
                  LCDDELAY1();
                  sendByte&=~E;                     //set E low
                  SPISEND(sendByte);
                  LCDDELAY1();

}

/*---------------------------------------------------
; Function Name: SPISEND
; Author: Capt Branchflower, USAF
; Function: Sends contents of r5 to SPI.
; Waits for Rx flag, clears by reading.
; Sets slave select accordingly.
; Outputs: none
; Registers destroyed: none
; Functions used: LCDWRT8, LCDDELAY1, LCDDELAY2
;---------------------------------------------------*/

void SPISEND(char byteToSend){

	              volatile char readByte;
				  SET_SS_LO();
                  UCB0TXBUF=byteToSend;         //transfer byte

                  while(!(UCB0RXIFG & IFG2))
                     {
                         // wait until you've received a byte
                     }
                  readByte = UCB0RXBUF;
                  SET_SS_HI();

                  }

/*---------------------------------------------------
; Function Name: cursorToLineTwo
; Author: C2C Payden McBee, USAFA
; Function:Sets the cursor on the LCD to line 2
; Outputs: none
; Registers destroyed: none
; Functions used: LCDWRT8, LCDDELAY1
;---------------------------------------------------*/
void cursorToLineTwo(){
	writeCommandByte(0xC0);

}

/*---------------------------------------------------
; Function Name: cursorToLineOne
; Author: C2C Payden McBee, USAFA
; Function:Sets the cursor on the LCD to line 1
; Outputs: none
; Registers destroyed: none
; Functions used: LCDWRT8, LCDDELAY1
;---------------------------------------------------*/
void cursorToLineOne(){
	writeCommandByte(0x80);
}

/*---------------------------------------------------
; Function Name: writeChar
; Author: C2C Payden McBee, USAFA
; Function:Writes a single character to the LCD
; Outputs: none
; Registers destroyed: none
; Functions used: LCDWRT8, LCDDELAY1
;---------------------------------------------------*/
void writeChar(char asciiChar){
	LCDCON |= RS_MASK;
	LCDWRT8(asciiChar);
    LCDDELAY1();
}
/*---------------------------------------------------
; Function Name: writeString
; Author: C2C Payden McBee, USAFA
; Function:Writes a string to the LCD
; Outputs: none
; Registers destroyed: none
; Functions used: writeChar
;---------------------------------------------------*/
void writeString(char string[]){
	int i=0;
	for (i = 0; i < 8; i++){
		writeChar(string[i]);
	}
}

/*---------------------------------------------------
; Function Name: scrollString
; Author: C2C Payden McBee, USAFA
; Function:scrolls and wraps a string to a line of the LCD
; Outputs: none
; Registers destroyed: none
; Functions used: cursorToLineOne, rotateString, writeString, cursorToLineTwo, longdelay, LCDCLR
;---------------------------------------------------*/
void scrollString(char string1[], char string2[]){
	while (1) //we want to scroll forever
	{
		cursorToLineOne();
		rotateString(string1);
		writeString(string1);
		cursorToLineTwo();
		rotateString(string2);
		writeString(string2);
		longdelay();
		LCDCLR();
	}
}
/*---------------------------------------------------
; Function Name: rotateString
; Author: C2C Payden McBee, USAFA
; Function: shifts the array members to the left and puts the first member at the end of the array, rotating it left
; Outputs: none
; Registers destroyed: none
; Functions used:findArrayLength
;---------------------------------------------------*/
void rotateString(char string[])
{
		int i=0;

		int arrayLength=findArrayLength(string);

		char firstVal=string[i];
		for(i=0;i<arrayLength;i++)
		{
			string[i]=string[i+1];
		}

		string[arrayLength-1] = firstVal;
}

/*---------------------------------------------------
; Function Name: findArrayLength
; Author: C2C Payden McBee, USAFA
; Function: finds the length of a given array
; Outputs: length of array
; Registers destroyed: none
; Functions used: none
;---------------------------------------------------*/
int findArrayLength(char string[])
{
	int j=0;
	int arrayLength=0;
	while(string[j]!=0)
			{
				arrayLength++;
				j++;
			}
	return arrayLength;
}

/*---------------------------------------------------
; Function Name: longdelay
; Author: C2C Payden McBee, USAFA
; Function: determines the speed of the scrolling, delays for 1.5 times lcddelay2, I played with the number until I liked the speed of the scrolling
; Outputs: none
; Registers destroyed: none
; Functions used: LCDDELAY2
;---------------------------------------------------*/
void longdelay()
{
	int j;
	for(j=0;j<150;j++)
	{
		LCDDELAY2();
	}
}

/*---------------------------------------------------
; Function Name: writeCommandNibble
; Author: Capt Todd Branchflower, USAF
; Function: Send command nibble to LCD, as opposed to a data nibble
; Outputs: none
; Registers destroyed: none
; Functions used: LCDWRT4
;---------------------------------------------------*/
void writeCommandNibble(char commandNibble)
{
		LCDCON&= ~RS_MASK;
		LCDWRT4(commandNibble);
		LCDDELAY2();

}
/*---------------------------------------------------
; Function Name: writeCommandByte
; Author: Capt Todd Branchflower, USAF
; Function: Send full byte to LCD telling it a command is about to come, not data
; Outputs: none
; Registers destroyed: none
; Functions used: LCDWRT4
;---------------------------------------------------*/
void writeCommandByte(char commandByte)
{
	    LCDCON &= ~RS_MASK;
	    LCDWRT8(commandByte);
	    LCDDELAY2();
}

void writeDataByte(char dataByte){
	LCDCON |= RS_MASK;
	LCDWRT8(dataByte);
	LCDDELAY2();
}





