#include <stdlib.h>

#include "map.h"
#include "../GLCD/GLCD.h"

enum MapCode {
	MC_Wall,
	MC_Pill,
	MC_Empty,
	MC_PowerPill,
};

int map_matrix[MAP_ROW_DIM][MAP_COL_DIM] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
{0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
{0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0},
{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
{0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0},
{0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0},
{0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
{0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
{0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 2, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
{0, 1, 1, 1, 1, 1, 1, 1, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1, 1, 0},
{0, 1, 0, 0, 1, 0, 0, 1, 0, 2, 2, 2, 0, 1, 0, 0, 1, 0, 0, 1, 0},
{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
{0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 2, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0},
{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
{0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0},
{0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0},
{0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

typedef struct pill_s
{
	uint8_t j;
	uint8_t i;
	uint8_t is_eaten;
	uint8_t is_power_pill;
} pill_t;

pill_t pills_array[TOTAL_PILLS];

int get_pill(int i, int j)
{
	int curr_pill;
	for (curr_pill = 0; curr_pill < TOTAL_PILLS; curr_pill++)
		if (pills_array[curr_pill].i == i && pills_array[curr_pill].j == j)
			return curr_pill;
	return -1;
}

// DRAW FUNCTIONS
// --------------
void draw_square_cell(uint16_t xpos, uint16_t ypos, uint16_t color, uint16_t dimension)
{
	int i;
	for(i = 0; i < dimension; i++)
	{
		LCD_DrawLine(xpos, ypos + i, xpos + dimension - 1, ypos + i, color);
	}
}

void draw_wall(int i, int j)
{
	draw_square_cell(X_OFFSET + 2 + j * CELL_DIM, Y_OFFSET + 2 + i * CELL_DIM, Blue, CELL_DIM - 4);
}

void draw_pill(int i, int j)
{
	draw_square_cell(X_OFFSET + (CELL_DIM - PILL_DIM)/2 + j * CELL_DIM, Y_OFFSET + (CELL_DIM - PILL_DIM)/2 + i * CELL_DIM, Yellow, PILL_DIM);
}
void draw_power_pill(int i, int j)
{
	draw_square_cell(X_OFFSET + (CELL_DIM - POWER_PILL_DIM)/2 + j * CELL_DIM, Y_OFFSET + (CELL_DIM - POWER_PILL_DIM)/2 + i * CELL_DIM, Yellow, POWER_PILL_DIM);
}

// MAP DRAW
// --------
void map_init()
{	
	int i,j,current_pill = 0;
	uint16_t block_color = Blue;
	
	for (i = 0; i < MAP_ROW_DIM; i++)
	{
		for (j = 0; j < MAP_COL_DIM; j++)
		{
			switch (map_matrix[i][j])
			{
				case (MC_Wall):
					draw_wall(i,j);
					break;
				case (MC_Pill):
					draw_pill(i,j);
					pills_array[current_pill].i = i;
					pills_array[current_pill].j = j;
					pills_array[current_pill].is_power_pill = 0;
					pills_array[current_pill].is_eaten = 0;
					current_pill++;
					break;
			}
		}
	}
}

void map_redraw_pause()
{
	int i,j;
	for (i = 11; i < 13; i++)
		for (j = 8; j < 13; j++)
			if (map_matrix[i][j] == MC_Wall)
				draw_wall(i,j);
}

void insert_power_pill()
{
	int random_pill_index;
	do {
		random_pill_index = rand() % (TOTAL_PILLS);
	} while (pills_array[random_pill_index].is_eaten == 1);
	
	pills_array[random_pill_index].is_power_pill = 1;
	map_matrix[pills_array[random_pill_index].i][pills_array[random_pill_index].j] = MC_PowerPill;
	draw_power_pill(pills_array[random_pill_index].i, pills_array[random_pill_index].j);
}

// EXTERNAL INTERFACE
// ------------------
void map_eat_pill(uint8_t j, uint8_t i)
{
	map_matrix[i][j] = MC_Empty;
	int16_t x, y;
	map_ji_to_xy(j, i, &x, &y);
	pills_array[get_pill(i, j)].is_eaten = 1;
	draw_square_cell(x, y, Black, CELL_DIM);
}

int map_is_pill(uint8_t j, uint8_t i)
{
	if (map_matrix[i][j] == MC_Pill)
		return 1;
	else if (map_matrix[i][j] == MC_PowerPill)
		return 2;
	return 0;
}

int map_is_wall(uint8_t j, uint8_t i)
{
	if (map_matrix[i][j] == MC_Wall)
		return 1;
	return 0;
}

int map_outofbound(int pacman_x)
{
	if (pacman_x > X_OFFSET + (MAP_COL_DIM - 2) * CELL_DIM + 5)
		return X_OFFSET + CELL_DIM - 5;
	else if (pacman_x < X_OFFSET + CELL_DIM - 5)
		return X_OFFSET + (MAP_COL_DIM - 2) * CELL_DIM + 5;
	return 0;
}

void map_xy_to_ji(int16_t x, int16_t y, int8_t *j, int8_t *i)
{
	if ((x - X_OFFSET) % 10 == 0)
		*j = (x - X_OFFSET) / CELL_DIM;
	
	if((y - Y_OFFSET) % 10 == 0)
		*i = (y - Y_OFFSET) / CELL_DIM;
}

void map_ji_to_xy(int8_t j, int8_t i, int16_t *x, int16_t *y)
{
	*x = X_OFFSET + j * CELL_DIM;
	*y = Y_OFFSET + i * CELL_DIM;
}