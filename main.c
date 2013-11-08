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
#define FIRST_ROW_SECOND_SPACE 0x81
#define SECOND_ROW_FIRST_SPACE 0xC0

#define CLEAR_LOWER_BIT 0x0f

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
				player=ON_FAR_LEFT;
				currentPos=ON_FAR_LEFT;
				hitMine=TRUE;
			}
			if(buttonPushed)
			{
				if(hitMine)
				{
					generateAndSetMines();
					hitMine=FALSE;
				}
				count=0; //reset counter
				if(didIwin)
				{
					didIwin=FALSE;
					generateAndSetMines();
				}
				else
				{
						LCDCLR();

						currentPos=movePlayer(currentPos, direction, onBottomRow,onTopRow, NotOnFarLeft, NotOnFarRight);
						printPlayer(currentPos);
						setMines(firstMine,secondMine);

						//set conditions to change boundary checks
						if(currentPos==ON_FAR_LEFT)
						{
							NotOnFarLeft=FALSE; NotOnFarRight= TRUE;
						}
						else
						{
							NotOnFarLeft=TRUE;
						}

						if(currentPos==ON_FAR_RIGHT)
						{
							NotOnFarRight=FALSE;
						}
						else
						{
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


						buttonPushed=FALSE; //reset button pushed

						//reset direction
						direction=noDirectionYet;
						//check if player won
						didIwin=didPlayerWin(currentPos);
				}
			}
			if (flag)  //this flag was set it the timer interrupt occurred, ie the timer overflowed
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

						buttonPushed=FALSE;

						//reset direction
						direction=noDirectionYet;
						//reset player position to starting point
						currentPos=startingPoint;

						displayYouWon();

						count=0;
					}

		} //ends while(1) loop
}	//ends main code

//this code creates mines which are not
void generateAndSetMines()
{
	unsigned char upper=0;
	unsigned char lower=0;

	//this makes sure the mines are not stacked nor adjacent to each other (diagonal)
	while(upper==lower || upper == (lower+1) || upper == (lower-1))
	{
		randomNum=prand(randomNum);
		firstMine= FIRST_ROW_SECOND_SPACE+randomNum%7;
		randomNum=prand(randomNum);
		secondMine=SECOND_ROW_FIRST_SPACE+randomNum%7;

		upper=(firstMine & CLEAR_LOWER_BIT);
		lower=(secondMine & CLEAR_LOWER_BIT);
	}

	setMines(firstMine,secondMine);
}

void init_timer()
{
	// do timer initialization work
		TACTL &= ~(MC1|MC0);        // stop timer

	    TACTL |= TACLR;             // clear TAR

	    TACTL |= TASSEL1;           // configure for SMCLK

	    TACTL |= ID1|ID0;           // divide timer clock by 8

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
//every 0xffff clock cycles, the timer overflows and the interrupt occurs
//the timer ticks one count every clock cycle
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
    TACTL &= ~TAIFG;            // clear interrupt flag
    flag = 1;
}


//interrrupts for buttons
//P1IES this is what triggers the interrupt, if it is 1, the interrupt will trigger on
//a falling edge. If it is 0, the interrupt will trigger on a rising edge. P1 Edge Select.
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
    if (P1IFG & BIT1)  //checks to see if bit1 of the interrupt flag is set (if bit1 is what triggered the interrupt)
    {
        P1IFG &= ~BIT1;                            // clear bit1 interrupt flag

        if (BIT1 & P1IES)		//if bit one is registered as a falling edge, react to button pushed
            {
        	        buttonPushed=TRUE;
        	        direction=goRight;
            }
        else					//if bit one is registered as a rising edge, debounce
            {
                     LCDDELAY2();//debounce
            }

        P1IES ^= BIT1; //flip the edge we want to trigger the interrupt on

    }

    if (P1IFG & BIT2) //checks to see if bit2 of the interrupt flag is set (if bit2 is what triggered the interrupt)
    {
        P1IFG &= ~BIT2;                         // clear bit2 interrupt flag

        if (BIT2 & P1IES)		//if bit two is registered as a falling edge, react to button pushed
            {
        			buttonPushed=TRUE;
        	        direction=goLeft;
            }
        else					//if bit two is registered as a rising edge, debounce
            {
                    LCDDELAY2();//debounce
            }

        P1IES ^= BIT2; //flip the edge we want to trigger the interrupt on

    }

    if (P1IFG & BIT3) //checks to see if bit3 of the interrupt flag is set (if bit3 is what triggered the interrupt)
    {
        P1IFG &= ~BIT3;                         // clear bit3 interrupt flag

        if (BIT3 & P1IES)	//if bit three is registered as a falling edge, react to button pushed
            {
        			buttonPushed=TRUE;
        	        direction=goUp;
            }
        else					//if bit three is registered as a rising edge, debounce
            {
                     LCDDELAY2();//debounce
            }

        P1IES ^= BIT3; //flip the edge we want to trigger the interrupt on

    }

    if (P1IFG & BIT4) //checks to see if bit4 of the interrupt flag is set (if bit4 is what triggered the interrupt)
       {
           P1IFG &= ~BIT4;                         // clear bit4 interrupt flag

           if (BIT4 & P1IES)	//if bit four is registered as a falling edge, react to button pushed
               {
        	        buttonPushed=TRUE;
        	        direction=goDown;
               }
           else					//if bit four is registered as a rising edge, debounce
               {
                    LCDDELAY2();//debounce
               }

         P1IES ^= BIT4;	//flip the edge we want to trigger the interrupt on

       }
}
