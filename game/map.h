
#include "LPC17xx.h"

#define MAP_ROW_DIM 24
#define MAP_COL_DIM 21

#define Y_OFFSET 40
#define X_OFFSET 15
#define CELL_DIM 10
#define PILL_DIM 2

void map_init();
void map_redraw_pause();

void map_eat_pill(uint8_t j, uint8_t i);
int map_is_pill(uint8_t j, uint8_t i);
int map_is_wall(uint8_t j, uint8_t i);

void map_xy_to_ji(int16_t x, int16_t y, int8_t *j, int8_t *i);
void map_ji_to_xy(int8_t j, int8_t i, int16_t *x, int16_t *y);