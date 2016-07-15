#include "input.h"

input::input()
{

}

input::~input()
{

}

void input::handle_event(SDL_Event e)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (!get_key(e.key.keysym.sym))
			pressed_now.insert(e.key.keysym.sym);
		keys_changed = true;
	} else if (e.type == SDL_KEYUP)
	{
		pressed_now.erase(e.key.keysym.sym);
		keys_changed = true;
	}
}

void input::next_frame()
{
	if (keys_changed)
	{
		pressed_prev = pressed_now;
		keys_changed = false;
	}
	
}

bool input::get_key(SDL_Keycode k)
{
	return pressed_now.find(k) != pressed_now.end();
}

bool input::get_key_down(SDL_Keycode k)
{
	return 
		pressed_prev.find(k) == pressed_prev.end()
		&& pressed_now.find(k) != pressed_now.end();
}

bool input::get_key_up(SDL_Keycode k)
{
	return
		pressed_prev.find(k) != pressed_prev.end()
		&& pressed_now.find(k) == pressed_now.end();
}
