#ifndef PACMAN_H
#define PACMAN_H

#include "LPC17xx.h"

#define PACMAN_HALF_DIM 4
#define PACMAN_DIM 8

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

void pacman_init();
void draw_pacman(uint16_t xpos, uint16_t ypos);
void pacman_new_direction(int dir);
void pacman_move();

#endif