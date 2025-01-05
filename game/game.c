#include "game.h"
#include "map.h"

#include "../GLCD/GLCD.h"
#include "../engine/input_handling.h"

#include <math.h>
#include <stdio.h>

// temporary for square display
#define PACMAN_HALF_DIM 4
#define PACMAN_DIM 8

// utility functions
void display_timer();
void display_score();

// GLOBAL VARIABLES
int game_state = GS_PLAY;
int high_score = 0;

int pacman_frames[4][PACMAN_DIM][PACMAN_DIM] = {
{
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
},
{
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 0, 0},
	{1, 1, 1, 1, 1, 0, 0, 0},
	{1, 1, 1, 1, 1, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
},
{
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 1, 0, 0, 0, 0},
	{1, 1, 1, 0, 0, 0, 0, 0},
	{1, 1, 1, 0, 0, 0, 0, 0},
	{0, 1, 1, 1, 0, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
},
{
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 0, 0},
	{1, 1, 1, 1, 1, 0, 0, 0},
	{1, 1, 1, 1, 1, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
}};


typedef struct pacman_s
{
	int16_t x;
	int16_t y;
	int8_t i;
	int8_t j;
	uint8_t direction;
	uint8_t speed; // every 50 ms
	uint8_t animation_frame;
} pacman_t;

pacman_t pacman;

void draw_pacman(uint16_t xpos, uint16_t ypos)
{
	int i,j;
	int i_start, i_end;
	int j_start, j_end;
	switch(pacman.direction)
	{
		case G_UP:
			for(i = PACMAN_DIM - 1; i >= 0; i--)
			{
				for(j = 0; j < PACMAN_DIM; j++)
				{
					if(pacman_frames[pacman.animation_frame][j][i])
						LCD_SetPoint(xpos + j, ypos + PACMAN_DIM - 1 - i, Yellow);
				}
			}
			break;
		case G_DOWN:
			for(i = 0; i < PACMAN_DIM; i++)
			{
				for(j = 0; j < PACMAN_DIM; j++)
				{
					if(pacman_frames[pacman.animation_frame][j][i])
						LCD_SetPoint(xpos + j, ypos + i, Yellow);
				}
			}
			break;
		case G_RIGHT:
			for(i = 0; i < PACMAN_DIM; i++)
			{
				for(j = 0; j < PACMAN_DIM; j++)
				{
					if(pacman_frames[pacman.animation_frame][i][j])
						LCD_SetPoint(xpos + j, ypos + i, Yellow);
				}
			}
			break;
		case G_LEFT:
			for(i = 0; i < PACMAN_DIM; i++)
			{
				for(j = PACMAN_DIM - 1; j >= 0; j--)
				{
					if(pacman_frames[pacman.animation_frame][i][j])
						LCD_SetPoint(xpos + PACMAN_DIM - 1 - j, ypos + i, Yellow);
				}
			}
			break;
	}
	
	pacman.animation_frame = (pacman.animation_frame + 1) % 4;
}

// GAME LOOP FUNCTIONS
void game_init()
{
	GUI_Text(10, 10, (uint8_t*) "TIME: ", White, Black);
	GUI_Text(110, 10, (uint8_t*) "HIGHSCORE: ", White, Black);

	pacman.i = 14;
	pacman.j = 10;
	map_ji_to_xy(pacman.j, pacman.i, &pacman.x, &pacman.y);
	pacman.direction = G_RIGHT;
	pacman.speed = 5;
	pacman.animation_frame = 0;
	map_init();
}

void game_process_input()
{
	// JOYSTICK INPUT ----
	if (J_isPressed(J_UP))
	{
		pacman_new_direction(G_UP);
	}
	else if (J_isPressed(J_RIGHT))
	{
		pacman_new_direction(G_RIGHT);
	}
	else if (J_isPressed(J_LEFT))
	{
		pacman_new_direction(G_LEFT);
	}
	else if (J_isPressed(J_DOWN))
	{
		pacman_new_direction(G_DOWN);
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

	
	if (current_tick == MAX_TIME)
	{
		change_game_state(GS_LOOSE);
	}

	pacman_move();
	map_xy_to_ji(pacman.x, pacman.y, &pacman.j, &pacman.i);
	
	if (map_is_pill(pacman.j, pacman.i) == 1) {
		map_eat_pill(pacman.j, pacman.i);
		high_score += 10;
	}
	
	// TODO: aggiornare con gli estremi della mappa
	/* CHECK BOUND LIMITS ------------------
	if (pacman.y >= MAX_Y + PACMAN_HALF_DIM)
		pacman.y = 0 - PACMAN_HALF_DIM;
	else if (pacman.y <= 0 - PACMAN_HALF_DIM)
		pacman.y = MAX_Y + PACMAN_HALF_DIM;
	
	if (pacman.x >= MAX_X + PACMAN_HALF_DIM)
		pacman.x = 0 - PACMAN_HALF_DIM;
	else if (pacman.x <= 0 - PACMAN_HALF_DIM)
		pacman.x = MAX_X + PACMAN_HALF_DIM;
	*/
}

// RENDER FUNCTIONS
void game_render()
{
	draw_pacman(pacman.x + 1, pacman.y + 1); // draw pacman
	display_timer();
	display_score();
}

// GAME STATE
void change_game_state(int new_state)
{
	switch (new_state)
	{
		case GS_PAUSE:
			game_state = GS_PAUSE;
			GUI_Text(100, 150, (uint8_t*)"PAUSE", White, Black);
			break;
		case GS_PLAY:
			game_state = GS_PLAY;
			GUI_Text(100, 150, (uint8_t*)"     ", Black, Black);
			map_redraw_pause();
			break;
		case GS_WIN:
			game_state = GS_WIN;
			GUI_Text(92, 150, (uint8_t*)"VICTORY!", White, Black);
			break;
		case GS_LOOSE:
			game_state = GS_LOOSE;
			GUI_Text(72, 150, (uint8_t*)"GAME OVER!", White, Black);
			break;
	}
}

// PACMAN LOGIC
void pacman_new_direction(int dir)
{
	switch (dir)
	{
		case G_UP:
			if(map_is_wall(pacman.j, pacman.i - 1) == 1)
				return;
			break;
		case G_DOWN:
			if(map_is_wall(pacman.j, pacman.i + 1) == 1)
				return;
			break;
		case G_RIGHT:
			if(map_is_wall(pacman.j + 1, pacman.i) == 1)
				return;
			break;
		case G_LEFT:
			if(map_is_wall(pacman.j - 1, pacman.i) == 1)
				return;
			break;
	}
	pacman.direction = dir;
}

void draw_square(uint16_t xpos, uint16_t ypos, uint16_t color)
{
	int i,j;
	for(i = 0; i < PACMAN_DIM; i++)
	{
		LCD_DrawLine(xpos, ypos + i, xpos + PACMAN_DIM - 1, ypos + i, color);
	}
}
	

void pacman_move()
{
	// MOVE PACMAN -----------------------
	draw_square(pacman.x + 1, pacman.y + 1, Black); // erase pacman
	switch (pacman.direction)
	{	
		case G_UP:
			if(map_is_wall(pacman.j, pacman.i - 1) == 0) 
				pacman.y -= pacman.speed;
			break;
		case G_DOWN:
			if(map_is_wall(pacman.j, pacman.i + 1) == 0) 
				pacman.y += pacman.speed;
			break;
		case G_RIGHT:
			if(map_is_wall(pacman.j + 1, pacman.i) == 0) 
				pacman.x += pacman.speed;
			break;
		case G_LEFT:
			if(map_is_wall(pacman.j - 1, pacman.i) == 0) 
				pacman.x -= pacman.speed;
			break;
	}
}

// UTILITIES
void display_timer() 
{
	float timer_shown = (float) (1200 - current_tick) * 0.05;
	char str[5]; 
	sprintf(str, "%.0f", ceilf(timer_shown));
	if ((1200 - current_tick + 1) % 120 == 0)
	{
		GUI_Text(56, 10, (uint8_t*) "   ", Black, Black);
		GUI_Text(56, 10, (uint8_t*) str, White, Black);
	}
}

void display_score() 
{
	char str[5]; 
	sprintf(str, "%4d", high_score);

	GUI_Text(198, 10, (uint8_t*) "       ", Black, Black);
	GUI_Text(198, 10, (uint8_t*) str, White, Black);

}