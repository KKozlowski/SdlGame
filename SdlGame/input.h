#pragma once
#include <SDL.h>
#include <set>
class input
{
private:
	std::set<SDL_Keycode> pressed_prev;
	std::set<SDL_Keycode> pressed_now;
	bool keys_changed = false;
public:
	input();
	~input();

	void handle_event(SDL_Event e);
	void next_frame();

	bool get_key(SDL_Keycode);
	bool get_key_down(SDL_Keycode);
	bool get_key_up(SDL_Keycode);
};
