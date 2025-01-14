#include "music.h"

#include "LPC17xx.h"
#include "../timer/timer.h"

#define N_SOUNDS 4

typedef struct sound_asset_c
{
	NOTE *sound;
	int curr_note;
	int len;
	int priority;
} sound_asset_t;
sound_asset_t sounds[N_SOUNDS];

sound_code_t current_sound = S_WIN;
int silence = 1;

NOTE win[] = {
  // 1
  {c4, time_semicroma},
  {c5, time_semicroma},
  {g4, time_semicroma},
  {e4, time_semicroma},
  {c5, time_semicroma},
  {g4, time_semicroma},
  {e4, time_semicroma},
  {pause, time_semicroma},
  //2
  {d4b, time_semicroma},
  {d5b, time_semicroma},
  {a4b, time_semicroma},
  {f4, time_semicroma},
  {d5b, time_semicroma},
  {a4b, time_semicroma},
  {f4, time_semicroma},
  {pause, time_semicroma},
  //3
  {c4, time_semicroma},
  {c5, time_semicroma},
  {g4, time_semicroma},
  {e4, time_semicroma},
  {c5, time_semicroma},
  {g4, time_semicroma},
  {e4, time_semicroma},
  {pause, time_semicroma},
  // 4
  {e4, time_biscroma},
  {f4, time_biscroma},
  {g4, time_biscroma},
  {pause, time_biscroma},
  {g4, time_biscroma},
  {a4b, time_biscroma},
  {a4, time_biscroma},
  {pause, time_biscroma},
  {a4, time_biscroma},
  {b4b, time_biscroma},
  {b4, time_biscroma},
  {pause, time_biscroma},
  {c5, time_croma},
  {pause, time_semicroma}
  
};

NOTE life[] = {
  {e5, time_semicroma},
  {g5, time_semicroma},
  {e6, time_semicroma},
  {c6, time_semicroma},
  {d6, time_semicroma},
  {g6, time_semicroma},
};

NOTE chomp[] = {
  {c5b, time_semibiscroma},
  {c5, time_semibiscroma},
  {pause, time_biscroma}
};

NOTE loose[] = {
	{e5, time_semibiscroma},
	{g5b, time_semibiscroma},
	{e5, time_semibiscroma},
	{e5b, time_semibiscroma},
	{e5, time_semibiscroma},
	{e5b, time_semibiscroma},
	{d5, time_semibiscroma},
	{e5b, time_semibiscroma},
	{d5, time_semibiscroma},
	{d5b, time_semibiscroma},
	{d5, time_semibiscroma},
	{d5b, time_semibiscroma},
	{c5, time_semibiscroma},
	{b4, time_semibiscroma},
	{b4b, time_semibiscroma},
	{f4b, time_semibiscroma},
	{g4b, time_semibiscroma},
	{g4b, time_semibiscroma},
};


void playSound()
{
	if (silence == 0)
		return;
	
	playNote(sounds[current_sound].sound[sounds[current_sound].curr_note++]);
	if(sounds[current_sound].curr_note >= sounds[current_sound].len)
		silence = 0;
}

void change_sound(sound_code_t new_sound)
{
	if(sounds[current_sound].priority >= sounds[new_sound].priority || silence == 0)
	{
		silence = 1;
		
		sounds[current_sound].curr_note = 0;
		current_sound = new_sound;
	}
}

void sound_init()
{
	sounds[S_WIN].sound = win;
	sounds[S_WIN].curr_note = 0;
	sounds[S_WIN].len = 38;
	sounds[S_WIN].priority = 0;
	
	sounds[S_LOOSE].sound = loose;
	sounds[S_LOOSE].curr_note = 0;
	sounds[S_LOOSE].len = 18;
	sounds[S_LOOSE].priority = 0;
	
	sounds[S_CHOMP].sound = chomp;
	sounds[S_CHOMP].curr_note = 0;
	sounds[S_CHOMP].len = 3;
	sounds[S_CHOMP].priority = 3;
	
	sounds[S_LIFE].sound = life;
	sounds[S_LIFE].curr_note = 0;
	sounds[S_LIFE].len = 6;
	sounds[S_LIFE].priority = 2;
}

void playNote(NOTE note)
{
	if(note.freq != pause)
	{
		reset_timer(1);
		init_timer(1, 0, 0, 3, note.freq);
		enable_timer(1);
	}
	reset_timer(2);
	init_timer(2, 0, 0, 7, note.duration);
	enable_timer(2);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM1->TCR != 0) || (LPC_TIM2->TCR != 0));
}