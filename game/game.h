#ifndef GAME_H
#define GAME_H

#include "LPC17xx.h"
#include <math.h>

// Global defines
#define TICK_FREQUENCY 20.0f			// time between two updates in ms
#define SCREEN_REFRESH_TICKS 10 	// render update time
#define MAX_TIME_SECONDS 60

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

extern int game_state;

// General render functions
void game_init();
void game_process_input();
void game_update();
void game_render();
void change_game_state(int new_state);

// PacMan functions
void pacman_new_direction(int dir);
void pacman_move();
void pacman_update_ji();

#endif