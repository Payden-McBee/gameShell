#include <msp430.h>
#include "game.h"
//#include "button.h"
#include "LCD.h"
//#include "rand.h"
#include "msp430-rng/rand.h"

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

int hitMine;


unsigned char noDirectionYet = 0x0;
unsigned char direction;

unsigned char currentPos;

unsigned char didIwin;

unsigned int randomNum;
unsigned int firstMine;
unsigned int secondMine;

 unsigned int upper;
 unsigned int lower;

void init_timer();
void init_buttons();

void generateAndSetMines();

volatile unsigned int seed;

void main(void)
{
	WDTCTL = (WDTPW|WDTHOLD);

	randomNum=rand();

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


	generateAndSetMines();

	//randomNum=prand(randomNum); //use in main program loop
	while(1)
	{
			if(currentPos==firstMine||currentPos==secondMine)
			{
				LCDCLR();
				BOOM();
				player=0x80;
				currentPos=0x80;
				hitMine=1;
				//generateAndSetMines();
				//player=gameOver(player);
				//generateAndSetMines();
			}
			if(buttonPushed)
			{
				if(hitMine)
				{
					generateAndSetMines();
					hitMine=0;
				}
				count=0; //reset timer
				if(didIwin)
				{
					didIwin=FALSE;
					generateAndSetMines();
				}
				else
				{
				LCDCLR();
				//setMines();
				currentPos=movePlayer(currentPos, direction, onBottomRow,onTopRow, NotOnFarLeft, NotOnFarRight);
				printPlayer(currentPos);
				setMines(firstMine,secondMine);
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
			        	   generateAndSetMines();
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

void generateAndSetMines()
{
	unsigned char upper=0;
	unsigned char lower=0;

	while(upper==lower || upper == (lower+1) || upper == (lower-1))
	{
		randomNum=prand(randomNum);
		firstMine= 0x81+randomNum%7;
		randomNum=prand(randomNum);
		secondMine=0xC0+randomNum%7;

		upper=(firstMine & 0x0f);
		lower=(secondMine & 0x0f);
	}

	setMines(firstMine,secondMine);
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
__interrupt void TIMER0_A1_ISR(void)
{
    TACTL &= ~TAIFG;            // clear interrupt flag
    flag = 1;
}


//interrrupts for buttons
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
