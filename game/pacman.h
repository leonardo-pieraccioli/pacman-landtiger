#ifndef PACMAN_H
#define PACMAN_H

#include "LPC17xx.h"

#define PACMAN_HALF_DIM 4
#define PACMAN_DIM 8

#define SPEED 5
#define INIT_I 14
#define INIT_J 10
#define INIT_ANIM_FRAME 0

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

// EXTERNAL VARIABLES
// ------------------
extern const int pacman_frames[4][PACMAN_DIM][PACMAN_DIM];
extern const int life_sprite[12][12];
extern pacman_t pacman;

void pacman_init();
void draw_pacman(uint16_t xpos, uint16_t ypos);
void pacman_new_direction(int dir);
void pacman_move();

#endif