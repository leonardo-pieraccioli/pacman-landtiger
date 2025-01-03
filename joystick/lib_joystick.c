/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
	/* joystick up functionality */
  LPC_PINCON->PINSEL3 &= ~(127<<20);	//PIN mode GPIO (00b value per P1.29) ALL JOYSTICK
	LPC_GPIO1->FIODIR   &= ~(15<<26);		//P1.25 Input (joysticks on PORT1 defined as Input) 
}
