/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../led/led.h"

#include "../GLCD/GLCD.h"
#include "../game/game.h"
#include "../music/music.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

static int currentNote = 0;
static int ticks = 0;

void RIT_IRQHandler (void)
{	
	/*music management*/
	if(!isNotePlaying()){
		++ticks;
		if(ticks == UPTICKS){
			ticks = 0;
			playSound();
		}
	}
	
	game_process_input();
	if(game_state == GS_PLAY)
	{
		game_update();	
		game_render();
	}
	// RESET RIT -------------------------------
  LPC_RIT->RICTRL |= 0x1;	/* clear RIT flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
