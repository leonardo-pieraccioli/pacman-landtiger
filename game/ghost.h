#ifndef GHOST_H
#define GHOST_H

#include "LPC17xx.h"

#define GHOST_HALF_DIM 4
#define GHOST_DIM 8

#define G_SPEED 1
#define G_INIT_I 11
#define G_INIT_J 10

typedef struct ghost_s
{
	int16_t x;
	int16_t y;
	int8_t i;
	int8_t j;
	uint8_t direction;
	uint8_t speed;
	uint8_t animation_frame;
} ghost_t;

void ghost_init();
void ghost_draw();
void ghost_update();
int find_path();
void move();

#endif