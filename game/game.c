
#include "game.h"
#include "pacman.h"
#include "map.h"

#include "../GLCD/GLCD.h"
#include "../engine/input_handling.h"
#include "CAN/CAN.h"
#include "timer.h"
#include "music/music.h"

#include <stdio.h>

#define LIVES_OFFSET 287

// FUNCTION DECLARATIONS
// ---------------------
void add_life();
void remove_life();

// GLOBAL VARIABLES
// ----------------
int game_state;
int high_score;
int lives;
int current_time;

int eaten_pills = 0;
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
	sound_init();
	
	change_game_state(GS_PAUSE);
	high_score = 0;
	lives = 1;
	current_time = MAX_TIME_SECONDS;
	
	tick_freq_in_seconds = TICK_FREQUENCY / 1000;
	ticks_per_second = (int) ceilf(1 / tick_freq_in_seconds);
	ticks_per_minute = ticks_per_second * MAX_TIME_SECONDS;
	
	GUI_Text(10, 10, (uint8_t*) "TIME: ", White, Black);
	GUI_Text(110, 10, (uint8_t*) "HIGHSCORE: ", White, Black);
	
	display_lives(lives);
	display_timer(current_time);
	display_score(high_score);
	
	pacman_init();
	
	game_render();
	
	change_sound(S_WIN);
	
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
	
	if (map_is_pill(pacman.j, pacman.i) != 0) {	
		if (map_is_pill(pacman.j, pacman.i) == 1) {	
			high_score += 10;
		}
		else if (map_is_pill(pacman.j, pacman.i) == 2) {
			high_score += 50;
		}		
		map_eat_pill(pacman.j, pacman.i);
		change_sound(S_CHOMP);
		eaten_pills++;
		
		send_game_status(current_time, lives, high_score);
			
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
			disable_timer(0);
			break;
		case GS_PLAY:
			game_state = GS_PLAY;
			GUI_Text(100, 150, (uint8_t*)"     ", Black, Black);
			map_redraw_pause();
			enable_timer(0);
			break;
		case GS_WIN:
			game_state = GS_WIN;
			GUI_Text(88, 150, (uint8_t*)"VICTORY!", White, Black);
			change_sound(S_WIN);
			disable_timer(0);
			break;
		case GS_LOOSE:
			game_state = GS_LOOSE;
			GUI_Text(80, 150, (uint8_t*)"GAME OVER!", White, Black);
			change_sound(S_LOOSE);
			disable_timer(0);
			break;
	}
}

// DISPLAY FUNCTIONS
// -----------------
void display_timer(int time) 
{
	char str[3]; 
	sprintf(str, "%d", time);
	GUI_Text(56, 10, (uint8_t*) "   ", Black, Black);
	GUI_Text(56, 10, (uint8_t*) str, White, Black);
}

void display_score(int score) 
{
	char str[5]; 
	sprintf(str, "%4d", score);

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

void display_lives(int lives)
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
	change_sound(S_LIFE);
}

void remove_life()
{
	int i, j;
	for(i = 0; i < 12; i++)
	{
		LCD_DrawLine(14 * lives, LIVES_OFFSET + i, 14 * lives + 12, LIVES_OFFSET + i, Black);
	}
	lives--;
	change_sound(S_LOOSE);
}

// CAN MESSAGING
// -------------
void send_game_status(int remaining_time, int remaining_lives, int score)
{
	char d0_score = score & 0xFF;
	char d1_score = (score & 0xFF00) >> 8;
	char d2_remaining_lives = remaining_lives & 0xFF;
	char d3_remaining_time = remaining_time & 0xFF;
	
	CAN_TxMsg.data[0] = d0_score;
	CAN_TxMsg.data[1] = d1_score;
	CAN_TxMsg.data[2] = d2_remaining_lives;
	CAN_TxMsg.data[3] = d3_remaining_time;
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 2;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg (1, &CAN_TxMsg);  

}