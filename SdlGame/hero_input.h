#pragma once
#include "actor.h"

class hero;
class input;

class hero_input : public actor
{
private:
	hero *m_hero;
	input *m_input;
public:
	SDL_Keycode left_key = SDLK_a;
	SDL_Keycode right_key = SDLK_d;
	SDL_Keycode up_key = SDLK_w;
	SDL_Keycode down_key = SDLK_s;

	SDL_Keycode dig_left_key = SDLK_q;
	SDL_Keycode dig_right_key = SDLK_e;

	hero_input(hero *h, input *i);
	void update() override;
};
