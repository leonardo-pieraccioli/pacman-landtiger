#ifndef GAME_H
#define GAME_H

#include "LPC17xx.h"

// Global defines
#define TICK_FREQUENCY 50 				// time between two updates in ms
#define SCREEN_REFRESH_TICKS 10 	// render update time
#define MAX_TIME 1200 // 60k ms, a tick every 50ms = 1200 tick for 60s of gameplay

// Tick 
static int current_tick = 0;

enum EntityDirections
{
	G_UP,
	G_RIGHT,
	G_DOWN,
	G_LEFT
};

enum GameStates
{
	GS_PLAY,
	GS_PAUSE,
	GS_WIN,
	GS_LOOSE
};

// General render functions
void game_init();
void game_process_input();
void game_update();
void game_render();
void change_game_state(int new_state);

// PacMan functions
void pacman_move(int dir);
void draw_square(uint16_t xpos, uint16_t ypos, uint16_t color);

#endif