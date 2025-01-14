/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through debounced buttons and Joystick
 *        	- key1 switches on the led at the left of the current led on, 
 *					- it implements a circular led effect,
 * 					- joystick UP function returns to initial configuration (led11 on) .
 * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "LPC17xx.h"                    /* LPC17xx definitions                */

#include "../adc/adc.h"
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "GLCD/GLCD.h"

#include "joystick/joystick.h"
#include "game/game.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
	SystemInit();  												/* System Initialization (i.e., PLL)  */
  
	CAN_Init();
	
	LCD_Initialization();
	LCD_Clear(Black);
	
	BUTTON_init();
	joystick_init();
	
	ADC_init();
	ADC_start_conversion();
	srand(read_AD_current());
	
	init_timer(0, 0, 0, 3, 0x017D7840);
	
	game_init();
	
	int rit_counter = TICK_FREQUENCY * 100000; // tick_f in ms, so multiplied by 100MHz but divided by 1000
	
	init_RIT(rit_counter);
	enable_RIT();
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
  while (1) {                           /* Loop forever                       */
		__ASM("wfi");
  }

}
