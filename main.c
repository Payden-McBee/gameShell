#include <msp430.h>
#include "game_shell/game.h"
#include "buttons/button.h"
#include "game_shell/LCD.h"


char startingPoint;

void init_timer();
void init_buttons();

void main(void)
{
	WDTCTL = (WDTPW|WDTHOLD);
	INITSPI();
	LCDINIT();
	LCDCLR();
	unsigned char player = initPlayer();

	init_timer();
	init_buttons();
	__enable_interrupt();
	startingPoint=initPlayer();
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

	}


}

//
// YOUR TIMER A ISR GOES HERE
//

void init_timer()
{
	// do timer initialization work
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
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
    if (P1IFG & BIT1)
    {
        P1IFG &= ~BIT1;                            // clear flag

    }

    if (P1IFG & BIT2)
    {
        P1IFG &= ~BIT2;                         // clear flag

    }

    if (P1IFG & BIT3)
    {
        P1IFG &= ~BIT3;                         // clear P1.3 interrupt flag

    }
    if (P1IFG & BIT4)
       {
           P1IFG &= ~BIT4;                         // clear P1.3 interrupt flag

       }
}
