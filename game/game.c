
#include "game.h"
#include "pacman.h"
#include "map.h"

#include "../GLCD/GLCD.h"
#include "../engine/input_handling.h"

#include <stdio.h>

#define LIVES_OFFSET 287

// FUNCTION DECLARATIONS
// ---------------------
void display_timer();
void display_score();
void display_lives();
void add_life();
void remove_life();

// GLOBAL VARIABLES
// ----------------
int game_state;
int high_score = 0;
int eaten_pills = 0;
int lives = 1;
int power_pill_spawn_times[6];
int last_power_pill_spawn = 0;

// RIT CONFIGURATION
// -----------------
float tick_freq_in_seconds;
int ticks_per_second;
int ticks_per_minute;

// GAME LOOP FUNCTIONS
// -------------------
void game_init()
{	
	map_init();
	change_game_state(GS_PAUSE);
	
	tick_freq_in_seconds = TICK_FREQUENCY / 1000;
	ticks_per_second = (int) ceilf(1 / tick_freq_in_seconds);
	ticks_per_minute = ticks_per_second * MAX_TIME_SECONDS;
	
	GUI_Text(10, 10, (uint8_t*) "TIME: ", White, Black);
	GUI_Text(110, 10, (uint8_t*) "HIGHSCORE: ", White, Black);
	
	display_lives();
	pacman_init();
	
	game_render();
	
	int i, curr_min = 0, curr_max = (ticks_per_second * 2) - 1;
	for (i = 0; i < 6; i++)
	{
		power_pill_spawn_times[i] = rand() % (curr_max - curr_min) + curr_min;
		curr_min += ticks_per_second * 2;
		curr_max += ticks_per_second * 2;
	}
	i++;
}

void game_process_input()
{
	// JOYSTICK INPUT
	// --------------
	if (J_isPressed(J_UP))
	{
		pacman_new_direction(G_UP);
	}
	else if (J_isPressed(J_RIGHT))
	{
		pacman_new_direction(G_RIGHT);
	}
	else if (J_isPressed(J_LEFT))
	{
		pacman_new_direction(G_LEFT);
	}
	else if (J_isPressed(J_DOWN))
	{
		pacman_new_direction(G_DOWN);
	}
	
	// BUTTON INPUT
	// ------------
	if (B_isPressed() == 1)
	{
		if(game_state == GS_PLAY)
			change_game_state(GS_PAUSE);
		else if (game_state == GS_PAUSE)
			change_game_state(GS_PLAY);
	}
}

void game_update()
{	
	current_tick++;
	if (current_tick == ticks_per_minute)
		change_game_state(GS_LOOSE);

	if (current_tick == power_pill_spawn_times[last_power_pill_spawn] && last_power_pill_spawn <= 6)
	{
		last_power_pill_spawn++;
		insert_power_pill();
	}
	
	pacman_move();
	map_xy_to_ji(pacman.x, pacman.y, &pacman.j, &pacman.i);
	
	if (map_is_pill(pacman.j, pacman.i) == 1) {
		map_eat_pill(pacman.j, pacman.i);
		eaten_pills++;
		high_score += 10;
		display_score();
		if (high_score >= 1000 * lives)
			add_life();
		if (eaten_pills == TOTAL_PILLS)
			change_game_state(GS_WIN);
	}
	else if (map_is_pill(pacman.j, pacman.i) == 2) {
		map_eat_pill(pacman.j, pacman.i);
		eaten_pills++;
		high_score += 50;
		display_score();
		if (high_score >= 1000 * lives)
			add_life();
		if (eaten_pills == TOTAL_PILLS)
			change_game_state(GS_WIN);
	}	
	
	int teleport_location = map_outofbound(pacman.x);
	if (teleport_location != 0)
	{
		pacman.x = teleport_location;
		if (pacman.x < 120)
			pacman.j = 0;
		else if (pacman.x > 120)
			pacman.j = MAP_COL_DIM - 1;
	}

}

void game_render()
{	
	draw_pacman(pacman.x + 1, pacman.y + 1); // draw pacman
	display_timer();
}

// GAME STATE
// ----------
void change_game_state(int new_state)
{
	switch (new_state)
	{
		case GS_PAUSE:
			game_state = GS_PAUSE;
			GUI_Text(100, 150, (uint8_t*)"PAUSE", White, Black);
			break;
		case GS_PLAY:
			game_state = GS_PLAY;
			GUI_Text(100, 150, (uint8_t*)"     ", Black, Black);
			map_redraw_pause();
			break;
		case GS_WIN:
			game_state = GS_WIN;
			GUI_Text(88, 150, (uint8_t*)"VICTORY!", White, Black);
			break;
		case GS_LOOSE:
			game_state = GS_LOOSE;
			GUI_Text(80, 150, (uint8_t*)"GAME OVER!", White, Black);
			break;
	}
}

// DISPLAY FUNCTIONS
// -----------------
void display_timer() 
{
	// 1200 a 50ms RIT, 3000 a 20ms

	
	float timer_shown = (float) (ticks_per_minute - current_tick) * tick_freq_in_seconds;
	char str[5]; 
	sprintf(str, "%.0f", timer_shown);
	if ((ticks_per_minute - current_tick + 1) % ticks_per_second == 0 || current_tick == 0)
	{
		GUI_Text(56, 10, (uint8_t*) "   ", Black, Black);
		GUI_Text(56, 10, (uint8_t*) str, White, Black);
	}
}

void display_score() 
{
	char str[5]; 
	sprintf(str, "%4d", high_score);

	GUI_Text(198, 10, (uint8_t*) "       ", Black, Black);
	GUI_Text(198, 10, (uint8_t*) str, White, Black);

}

void draw_life(int offset)
{
	int i, j;
	for(i = 0; i < 12; i++)
	{
		for(j = 0; j < 12; j++)
		{
			if(life_sprite[i][j])
				LCD_SetPoint(14 * offset + j, LIVES_OFFSET + i, Yellow);
		}
	}
}

void display_lives()
{
	int k;
	for (k = 1; k <= lives; k++)
	{
		draw_life(k);
	}
}

// LIVES LOGIC
// -----------

void add_life()
{
	lives++;
	draw_life(lives);
}

void remove_life()
{
	int i, j;
	for(i = 0; i < 12; i++)
	{
		LCD_DrawLine(14 * lives, LIVES_OFFSET + i, 14 * lives + 12, LIVES_OFFSET + i, Black);
	}
	lives--;
}