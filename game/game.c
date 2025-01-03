#include "game.h"
#include "../GLCD/GLCD.h"
#include "../engine/input_handling.h"
#include <math.h>
#include <stdio.h>

// temporary for square display
#define PACMAN_DIM 9
#define PACMAN_HALF_DIM 5

// utility functions
void display_timer();

// GLOBAL VARIABLES
int game_state = GS_PLAY;

typedef struct pacman_s
{
	int16_t x;
	int16_t y;
	uint8_t direction;
	uint8_t speed; // every 50 ms
} pacman_t;
pacman_t pacman;


// GAME LOOP FUNCTIONS
void game_init()
{
	pacman.x = 50;
	pacman.y = 50;
	pacman.direction = G_RIGHT;
	pacman.speed = 5;
}

void game_process_input()
{
	// JOYSTICK INPUT ----
	if (J_isPressed(J_UP))
	{
		pacman_move(G_UP);
	}
	else if (J_isPressed(J_RIGHT))
	{
		pacman_move(G_RIGHT);
	}
	else if (J_isPressed(J_LEFT))
	{
		pacman_move(G_LEFT);
	}
	else if (J_isPressed(J_DOWN))
	{
		pacman_move(G_DOWN);
	} 
	else // If the joystick is centered
	{
		
	}
	
	// BUTTON INPUT---
	if (B_isPressed())
	{
		if(game_state == GS_PLAY)
			change_game_state(GS_PAUSE);
		else if (game_state == GS_PAUSE)
			change_game_state(GS_PLAY);
	}
}

void game_update()
{
	if (game_state != GS_PLAY) return;
	
	current_tick++;
	display_timer();
	if (current_tick == MAX_TIME)
	{
		change_game_state(GS_LOOSE);
	}
	
	// MOVE PACMAN -----------------------
	draw_square(pacman.x, pacman.y, Blue);
	switch (pacman.direction)
	{	
		case G_UP:
			pacman.y -= pacman.speed;
			break;
		case G_DOWN:
			pacman.y += pacman.speed;
			break;
		case G_RIGHT:
			pacman.x += pacman.speed;
			break;
		case G_LEFT:
			pacman.x -= pacman.speed;
			break;
	}
	draw_square(pacman.x, pacman.y, Yellow);
	
	// CHECK BOUND LIMITS ------------------
	if (pacman.y >= MAX_Y + PACMAN_HALF_DIM)
		pacman.y = 0 - PACMAN_HALF_DIM;
	else if (pacman.y <= 0 - PACMAN_HALF_DIM)
		pacman.y = MAX_Y + PACMAN_HALF_DIM;
	
	if (pacman.x >= MAX_X + PACMAN_HALF_DIM)
		pacman.x = 0 - PACMAN_HALF_DIM;
	else if (pacman.x <= 0 - PACMAN_HALF_DIM)
		pacman.x = MAX_X + PACMAN_HALF_DIM;
}

// RENDER FUNCTIONS
void game_render()
{
	
}

void draw_square(uint16_t xpos, uint16_t ypos, uint16_t color)
{
	int i,j;
	int xpos_draw, ypos_draw = ypos - (uint16_t) PACMAN_HALF_DIM;
	for(i=0; i<10; i++)
	{
		xpos_draw = xpos - (uint16_t) PACMAN_HALF_DIM;
		for(j=0; j<10; j++)
		{
			LCD_SetPoint(xpos_draw++, ypos_draw, color);
		}
		ypos_draw++;
	}
}


// GAME STATE
void change_game_state(int new_state)
{
	switch (new_state)
	{
		case GS_PAUSE:
			game_state = GS_PAUSE;
			GUI_Text(50, 100, (uint8_t*)"PAUSE", White, Black);
			break;
		case GS_PLAY:
			game_state = GS_PLAY;
			GUI_Text(50, 100, (uint8_t*)"     ", Blue, Blue);
			break;
		case GS_WIN:
			game_state = GS_WIN;
			GUI_Text(50, 100, (uint8_t*)"VICTORY!", White, Black);
			break;
		case GS_LOOSE:
			game_state = GS_LOOSE;
			GUI_Text(50, 100, (uint8_t*)"GAME OVER!", White, Black);
			break;
	}
}

// PACMAN LOGIC
void pacman_move(int dir)
{
	pacman.direction = dir;
}

// UTILITIES
void display_timer() 
{
	float timer_shown = (float) (1200 - current_tick) * 0.05;
	char str[5]; 
	sprintf(str, "%.2f", timer_shown);
	GUI_Text(0, 0, (uint8_t*) "   ", Blue, Blue);
	GUI_Text(0, 0, (uint8_t*) str, White, Black);
}