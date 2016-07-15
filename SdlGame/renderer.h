#pragma once
#include <SDL_image.h>
#include "draw_base.h"
#include <set>
#include <cstdio>


class renderer
{
private:
	SDL_Renderer* mainRenderer = nullptr;
	SDL_Window* mainWindow = nullptr;
	SDL_Surface* screenSurface = nullptr;
	static bool sdl_initialized;

	std::set<draw_base *> *drawed;
public:
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	static bool initialize_sdl();
	static void close_sdl();
	renderer();
	~renderer();

	SDL_Renderer *get_renderer() const;
	SDL_Window *get_window() const;

	void clear();
	void clear(Uint8 r, Uint8 g, Uint8 b);
	void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	bool add_draw(draw_base *);
	bool remove_draw(draw_base *);

	void draw();
};
