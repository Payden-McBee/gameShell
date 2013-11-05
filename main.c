#include <msp430.h>
#include "game_shell/game.h"
#include "buttons/button.h"
#include "game_shell/LCD.h"

#define TRUE 0x1
#define FALSE 0x0

#define ON_FAR_RIGHT 0x87
#define ON_FAR_LEFT 0x80
#define LAST_SPOT_TOP_ROW 0X88

unsigned char startingPoint;
unsigned char buttonPushed;

unsigned char onTopRow;
unsigned char onBottomRow;
unsigned char NotOnFarRight;
unsigned char NotOnFarLeft;

unsigned char goRight=0x1;
unsigned char goLeft=0x2;
unsigned char goUp=0x3;
unsigned char goDown=0x4;

int flag;
int count;
unsigned char timeExpired;



unsigned char noDirectionYet = 0x0;
unsigned char direction;

unsigned char currentPos;

unsigned char didIwin;

void init_timer();
void init_buttons();

void main(void)
{
	WDTCTL = (WDTPW|WDTHOLD);
	INITSPI();
	LCDINIT();
	LCDCLR();
	unsigned char player = initPlayer();

	flag=0;
	count=0;

	init_timer();
	init_buttons();
	__enable_interrupt();
	startingPoint=initPlayer();
	onTopRow=TRUE;
	onBottomRow=FALSE;
	NotOnFarRight=TRUE;
	NotOnFarLeft=FALSE;

	timeExpired=FALSE;

	//start out going in no particular direction
	buttonPushed=FALSE;
	direction=noDirectionYet;
	/////////////every time you detect a release (rising edge) delay for a short period of time, this should debounce it
	currentPos=startingPoint;
	printPlayer(startingPoint);
	while(1)
	{
		/*
		 * while (game is on)
		 * {
		 * 		check if button is pushed (you don't want to block here, so don't poll!)
		 * 		if button is pushed:
		 * 			clear current player marker
		 * 			update player position based on direction
		 * 			print new player
		 * 			clear two second timer
		 * 			wait for button release (you can poll here)
		 * }
		 *
		 * print game over or you won, depending on game result
		 *
		 * wait for button press to begin new game (you can poll here)
		 * wait for release before starting again
		 */
			if(buttonPushed)
			{
				count=0; //reset timer
				if(didIwin)
				{
					didIwin=FALSE;
				}
				else
				{

				//do something
				LCDCLR();
				currentPos=movePlayer(currentPos, direction, onBottomRow,onTopRow, NotOnFarLeft, NotOnFarRight);
				printPlayer(currentPos);
				//set conditions to change boundary checks
				if(currentPos==80)
					{
					NotOnFarLeft=FALSE; NotOnFarRight= TRUE;

					}
				else{
					NotOnFarLeft=TRUE;
				}
				if(currentPos==0x87)
				{
					NotOnFarRight=FALSE;
				}
				else{
					NotOnFarRight=TRUE;
				}
				if(currentPos>LAST_SPOT_TOP_ROW)
				{
					onTopRow=FALSE;
					onBottomRow=TRUE;
				}
				else
				{
					onTopRow=TRUE;
					onBottomRow=FALSE;
				}

				//then change boundary if on far right or left then both ok
				//then win the game if on far bottom right, if statement with capn B's function didWin
				buttonPushed=FALSE; //reset button pushed
				 //reset directions
				direction=noDirectionYet;
				didIwin=didPlayerWin(currentPos);
			}
			}
			if (flag)
			        {
			            flag = 0;
			           count++;
			           if(count>6)
			           {
			        	   //display gameOver  and reinitialize game
			        	   player=0x80;
			        	   player=gameOver(player);

			           }

			        }
			if(didIwin)
							{
								LCDCLR();
								startingPoint=initPlayer();
								onTopRow=TRUE;
								onBottomRow=FALSE;
								NotOnFarRight=TRUE;
								NotOnFarLeft=FALSE;

									//start out going in no particular direction
								buttonPushed=FALSE;
								direction=noDirectionYet;

								currentPos=startingPoint;

								char string1[]="You       ";
								char string2[]="Won!      ";
								cursorToLineOne();
								writeString(string1);
								cursorToLineTwo();
								writeString(string2);
								count=0;
							}
	}


}

//
// YOUR TIMER A ISR GOES HERE
//

void init_timer()
{
	// do timer initialization work
		TACTL &= ~(MC1|MC0);        // stop timer

	    TACTL |= TACLR;             // clear TAR

	    TACTL |= TASSEL1;           // configure for SMCLK - what's the frequency (roughly)?

	    TACTL |= ID1|ID0;           // divide clock by 8 - what's the frequency of interrupt?

	    TACTL &= ~TAIFG;            // clear interrupt flag

	    TACTL |= MC1;               // set count mode to continuous

	    TACTL |= TAIE;              // enable interrupt
}

void init_buttons()
{
	// do button initialization work
	 P1DIR &= ~(BIT1|BIT2|BIT3|BIT4);                // set buttons to input
	 P1IE |= BIT1|BIT2|BIT3|BIT4;                    // enable the interrupts
	 P1IES |= BIT1|BIT2|BIT3|BIT4;                   // configure interrupt to sense falling edges
	 P1REN |= BIT1|BIT2|BIT3|BIT4;                   // enable internal pull-up/pull-down network
	 P1OUT |= BIT1|BIT2|BIT3|BIT4;                   // configure as pull-up
	 P1IFG &= ~(BIT1|BIT2|BIT3|BIT4);                // clear flags
}
// Flag for continuous counting is TAIFG
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR()
{
    TACTL &= ~TAIFG;            // clear interrupt flag
    flag = 1;
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
    if (P1IFG & BIT1)
    {
        P1IFG &= ~BIT1;                            // clear flag

        if (BIT1 & P1IES)
            {
        	        buttonPushed=TRUE;
        	        direction=goRight;
            }
        else
            {
                     LCDDELAY2();//debounce
            }

        P1IES ^= BIT1;
        P1IFG &= ~BIT1;
    }

    if (P1IFG & BIT2)
    {
        P1IFG &= ~BIT2;                         // clear flag

        if (BIT2 & P1IES)
            {
        			buttonPushed=TRUE;
        	        direction=goLeft;
            }
        else
            {
                    LCDDELAY2();//debounce
            }

        P1IES ^= BIT2;
        P1IFG &= ~BIT2;
    }

    if (P1IFG & BIT3)
    {
        P1IFG &= ~BIT3;                         // clear P1.3 interrupt flag

        if (BIT3 & P1IES)
            {
        			buttonPushed=TRUE;
        	        direction=goUp;
            }
        else
            {
                     LCDDELAY2();//debounce
            }

        P1IES ^= BIT3;
        P1IFG &= ~BIT3;
    }

    if (P1IFG & BIT4)
       {
           P1IFG &= ~BIT4;                         // clear P1.3 interrupt flag

           if (BIT4 & P1IES)
               {
        	        buttonPushed=TRUE;
        	        direction=goDown;
               }
           else
               {
                    LCDDELAY2();//debounce
               }

         P1IES ^= BIT4;
         P1IFG &= ~BIT4;
       }
}
