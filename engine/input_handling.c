#include "input_handling.h"
#include "LPC17xx.h"


void J_cleanInputsOtherThan(int active_key);

static int J_up = 0;			//FIOPIN 29
static int J_right = 0; 	//FIOPIN 28
static int J_left = 0;  	//FIOPIN 27
static int J_down = 0;		//FIOPIN 26

volatile int INT0_down=0;

int J_isPressed(int j_key)
{
	int fiopin_offset = 1;
	int *active_key;
	switch(j_key)
	{
		case J_UP: //UP
			fiopin_offset = fiopin_offset << 29;
			active_key = &J_up;
			break;
		case J_RIGHT: //RIGHT
			fiopin_offset = fiopin_offset << 28;
			active_key = &J_right;
			break;
		case J_LEFT: //LEFT
			fiopin_offset = fiopin_offset << 27;
			active_key = &J_left;
			break;
		case J_DOWN: //DOWN
			fiopin_offset = fiopin_offset << 26;
			active_key = &J_down;
			break;
	}
	
  if((LPC_GPIO1->FIOPIN & fiopin_offset) == 0)
	{	
		/* Joystick direction pressed */
		*active_key = (*active_key) + 1;
		if((*active_key) >= 1)   
		{
			return 1;
		}
	}
	else 
	{
		J_cleanInputsOtherThan(*active_key);
	}
	return 0; //FALSE
}

void J_cleanInputsOtherThan(int direction)
{
	if (direction != J_UP) { J_up = 0; }
	if (direction != J_RIGHT) { J_right = 0; }
	if (direction != J_LEFT) { J_left = 0; }
	if (direction != J_DOWN) { J_down = 0; }
}

int B_isPressed()
{
	// BUTTON INPUT -------------------
	if(INT0_down>=1)
	{ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* KEY0 pressed */
			if(INT0_down >= 1)
				return 1;
		}
		else 
		{	/* button released */
			INT0_down=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	return 0;
}