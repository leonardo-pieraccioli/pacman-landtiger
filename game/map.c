#include "map.h"
#include "../GLCD/GLCD.h"
enum MapCode {
	MC_Wall,
	MC_Pill,
	MC_Empty,
	MC_PacMan
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

void draw_square_cell(uint16_t xpos, uint16_t ypos, uint16_t color, uint16_t dimension)
{
	int i,j;
	for(i = 0; i < dimension; i++)
	{
		LCD_DrawLine(xpos, ypos + i, xpos + dimension - 1, ypos + i, color);
	}
}
	
void map_init()
{
	int i,j;
	uint16_t block_color = Blue;
	
	for (i = 0; i < MAP_ROW_DIM; i++)
	{
		for (j = 0; j < MAP_COL_DIM; j++)
		{

			switch (map_matrix[i][j])
			{
				case (MC_Wall):
					draw_square_cell(X_OFFSET + 2 + j * CELL_DIM, Y_OFFSET + 2 + i * CELL_DIM, Blue, CELL_DIM - 4);
					
					break;
				case (MC_Pill):
					draw_square_cell(X_OFFSET + (CELL_DIM - PILL_DIM)/2 + j * CELL_DIM, Y_OFFSET + (CELL_DIM - PILL_DIM)/2 + i * CELL_DIM, Yellow, PILL_DIM);
					
					break;
				default:
					
					break;
			}
		}
	}
}

void map_redraw_pause()
{
	int i,j;
	for (i = 11; i < 13; i++)
	{
		for (j = 8; j < 13; j++)
		{
			switch (map_matrix[i][j])
			{
				case (MC_Wall):
					draw_square_cell(X_OFFSET + 2 + j * CELL_DIM, Y_OFFSET + 2 + i * CELL_DIM, Blue, CELL_DIM - 4);
					
					break;

				default:
					
					break;
			}
		}
	}
}

void map_eat_pill(uint8_t j, uint8_t i)
{
	map_matrix[i][j] = MC_Empty;
}
int map_is_pill(uint8_t j, uint8_t i)
{
	if (map_matrix[i][j] == MC_Pill)
		return 1;
	return 0;
}

int map_is_wall(uint8_t j, uint8_t i)
{
	if (map_matrix[i][j] == MC_Wall)
		return 1;
	return 0;
}

void map_xy_to_ji(int16_t x, int16_t y, int8_t *j, int8_t *i)
{
	if ((x - X_OFFSET) % 10 == 0)
		*j = (x - X_OFFSET) / CELL_DIM;
	
	if((y - Y_OFFSET) % 10 == 0)
		*i = (y - Y_OFFSET) / CELL_DIM;
}

int map_outofbound(int pacman_x)
{
	if (pacman_x > X_OFFSET + (MAP_COL_DIM - 2) * CELL_DIM + 5)
		return X_OFFSET + CELL_DIM - 5;
	else if (pacman_x < X_OFFSET + CELL_DIM - 5)
		return X_OFFSET + (MAP_COL_DIM - 2) * CELL_DIM + 5;
	return 0;
}

void map_ji_to_xy(int8_t j, int8_t i, int16_t *x, int16_t *y)
{
	*x = X_OFFSET + j * CELL_DIM;
	*y = Y_OFFSET + i * CELL_DIM;
}