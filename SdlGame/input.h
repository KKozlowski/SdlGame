#pragma once
#include <SDL.h>
#include <set>
#include "vector2.h"

class input
{
private:
	std::set<SDL_Keycode> pressed_prev;
	std::set<SDL_Keycode> pressed_now;
	bool keys_changed = false;

	point mouse_pos;
	uint32_t mouse_state;
public:
	input();
	~input();

	///<summary>
	///Handles input events passed through the parameter.
	///</summary>
	void handle_event(SDL_Event e);

	///<summary>
	///Reloads input info. Should be called once per frame.
	///</summary>
	void next_frame();

	///<summary>
	///Checks if a given keyboard key is pressed in current frame.
	///</summary>
	bool get_key(SDL_Keycode);

	///<summary>
	///Checks if a given keyboard key was just pressed 
	///(is pressed in current frame, but not in the previous).
	///</summary>
	bool get_key_down(SDL_Keycode);

	///<summary>
	///Checks if a given keyboard key was just released 
	///(is not pressed in current frame, but was pressed in the previous).
	///</summary>
	bool get_key_up(SDL_Keycode);

	///<summary>
	///Returns current mouse cursor position relative to window bounds.
	///It is not updated when the cursor is outside the window.
	///</summary>
	point get_mouse_position() const;
	
	///<summary>
	///Checks if a given mouse key is pressed in current frame.
	///1 - LEFT, 2 - MIDDLE, 3 - RIGHT
	///</summary>
	bool get_mouse_key(uint16_t) const;
};
