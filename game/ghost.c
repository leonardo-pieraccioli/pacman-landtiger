#include "ghost.h"
#include "map.h"
#include "GLCD/GLCD.h"
#include "math.h"
#include "pacman.h"

ghost_t ghost;

uint8_t ghost_sprite[2][GHOST_DIM][GHOST_DIM] = {
	{
		{0, 1, 1, 1, 1, 1, 1, 0},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 2, 3, 1, 2, 3, 1},
		{1, 1, 2, 3, 1, 2, 3, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 0, 1, 0, 1, 0, 1}
	},
	{
		{0, 1, 1, 1, 1, 1, 1, 0},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 2, 3, 1, 2, 3, 1},
		{1, 1, 2, 3, 1, 2, 3, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 1, 0, 1, 0, 1, 0}
	}
};

typedef struct 
{
	int i;
	int j;
	int n_neighbours;
	
} node_t;

node_t walkable_cells[258];


void ghost_init()
{
	ghost.i = G_INIT_I;
	ghost.j = G_INIT_J;
	map_ji_to_xy(ghost.j, ghost.i, &ghost.x, &ghost.y);
	ghost.speed = G_SPEED;
	ghost.direction = G_UP;
	ghost.animation_frame = 0;
	
	int count = count_walk_cells();
	
	
}

void draw_black_square(uint16_t xpos, uint16_t ypos)
{
	int i,j;
	for(i = 0; i < GHOST_DIM; i++)
	{
		LCD_DrawLine(xpos, ypos + i, xpos + GHOST_DIM - 1, ypos + i, Black);
	}
}

void ghost_draw()
{
	if (map_is_pill(ghost.j, ghost.i) == 1)
		draw_pill(ghost.i, ghost.j);
	if (map_is_pill(ghost.j, ghost.i) == 2)
		draw_power_pill(ghost.i, ghost.j);
	
	int i,j;
	for (i = 0; i < GHOST_DIM; i++)
	{
		for (j = 0; j < GHOST_DIM; j++)
		{
			switch(ghost_sprite[ghost.animation_frame][i][j])
			{
				case 0:
					break;
				case 1:
					LCD_SetPoint(ghost.x + 1 + j, ghost.y + 1 + i, Red);
					break;
				case 2:
					LCD_SetPoint(ghost.x + 1 + j, ghost.y + 1 + i, White);
					break;
				case 3:
					LCD_SetPoint(ghost.x + 1 + j, ghost.y + 1 + i, Black);
					break;
			}
		}
	}
	ghost.animation_frame = (ghost.animation_frame + 1) % 2;
}

void ghost_update() 
{
	draw_black_square(ghost.x + 1, ghost.y + 1);
	
	map_xy_to_ji(ghost.x, ghost.y, &ghost.j, &ghost.i);
	
	switch (ghost.direction)
	{
		case G_UP:
			if(map_is_wall(ghost.j, ghost.i - 1) == 0) 
				ghost.y -= ghost.speed;
			break;
		case G_DOWN:
			if(map_is_wall(ghost.j, ghost.i + 1) == 0) 
				ghost.y += ghost.speed;
			break;
		case G_RIGHT:
			if(map_is_wall(ghost.j + 1, ghost.i) == 0) 
				ghost.x += ghost.speed;
			break;
		case G_LEFT:
			if(map_is_wall(ghost.j - 1, ghost.i) == 0) 
				ghost.x -= ghost.speed;
			break;
	}
	
	if((ghost.x - X_OFFSET) % CELL_DIM != 0 || (ghost.y - Y_OFFSET) % CELL_DIM != 0)
		return;
	
	ghost.direction = find_path();
}

float distance(int i1, int j1, int i2, int j2)
{
	return sqrt(powf(j2 - j1, 2) + powf(i2 - i1, 2));
}

int find_path()
{
	int i, new_direction, min_distance = 1000000;
	int new_distance;
	if (!map_is_wall(ghost.j, ghost.i - 1))
	{	
		new_distance = distance(ghost.i - 1, ghost.j, pacman.i, pacman.j);
		if (new_distance < min_distance)
		{
			min_distance = new_distance;
			new_direction = G_UP;
		}
	}
	if (!map_is_wall(ghost.j + 1, ghost.i))
	{
		new_distance = distance(ghost.i, ghost.j + 1, pacman.i, pacman.j);
		if (distance(ghost.i, ghost.j + 1, pacman.i, pacman.j) < min_distance)
		{
			min_distance = new_distance;
			new_direction = G_RIGHT;
		}
	}
	
	if (!map_is_wall(ghost.j, ghost.i + 1))
	{
		new_distance = distance(ghost.i + 1, ghost.j, pacman.i, pacman.j);
		if (new_distance < min_distance)
		{
			min_distance = new_distance;
			new_direction = G_DOWN;
		}
	}
	
	if (!map_is_wall(ghost.j - 1, ghost.i))
	{	
		new_distance = distance(ghost.i, ghost.j - 1, pacman.i, pacman.j);
		if (new_distance < min_distance)
		{
			min_distance = new_distance;
			new_direction = G_LEFT;
		}
	}
	
	return new_direction;
}
