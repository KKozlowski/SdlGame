#pragma once
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "draw_base.h"
#include <set>
#include <cstdio>
#include <vector>

class camera;


///<summary>
/// Class that calles draw() on all the visible draw_base objects. <para />
/// It holds the SDL_Renderer and SDL_Window objects, cotrolling
/// all the graphics oriented stuff.
///</summary>
class renderer
{
private:
	SDL_Renderer* m_sdlRenderer = nullptr;
	SDL_Window* m_window = nullptr;
	static bool sdl_initialized;

	TTF_Font* m_font = nullptr;

	camera *m_camera;

	std::set<draw_base *> *m_draws;
public:
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	static bool initialize_sdl();
	static void close_sdl();
	renderer();
	~renderer();

	SDL_Renderer *get_renderer() const;
	SDL_Window *get_window() const;
	camera *get_camera() const;

	void print_text(std::string content, point position, int height);

	void clear();
	void clear(Uint8 r, Uint8 g, Uint8 b);
	void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	///<summary>Add a draw_base object to the list of objects drawn every frame. </summary>
	bool add_draw(draw_base *);
	///<summary>Remove a draw_base object from the list of objects drawn every frame. </summary>
	bool remove_draw(draw_base *);

	///<summary>Text that is displayed at the bottom of the screen. Good for score displaying. </summary>
	std::string bottom_text = "";

	void draw();
};
