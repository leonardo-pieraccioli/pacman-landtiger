#include "pacman.h"
#include "../GLCD/GLCD.h"
#include "game.h"

const int pacman_frames[4][PACMAN_DIM][PACMAN_DIM] = {
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
	{0, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 0, 0, 0},
	{1, 1, 1, 1, 1, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
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
	{0, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 0, 0, 0},
	{1, 1, 1, 1, 1, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
}};

const int life_sprite[12][12] = {
	{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
	{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
};

pacman_t pacman;

void pacman_init()
{
	pacman.i = 9;
	pacman.j = 18;
	map_ji_to_xy(pacman.j, pacman.i, &pacman.x, &pacman.y);
	
	pacman.direction = G_RIGHT;
	pacman.speed = 5;
	pacman.animation_frame = 0;
	
}

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