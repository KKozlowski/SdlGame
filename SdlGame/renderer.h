﻿#pragma once
#include <SDL_image.h>
#include "draw_base.h"
#include <set>
#include <cstdio>

class camera;
struct cmpStruct;

class renderer
{
private:
	SDL_Renderer* mainRenderer = nullptr;
	SDL_Window* mainWindow = nullptr;
	SDL_Surface* screenSurface = nullptr;
	static bool sdl_initialized;

	camera *m_camera;

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
	camera *get_camera() const;

	void clear();
	void clear(Uint8 r, Uint8 g, Uint8 b);
	void set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	bool add_draw(draw_base *);
	bool remove_draw(draw_base *);

	void draw();
};

struct cmpStruct {
	bool operator() (draw_base * const & lhs, draw_base * const & rhs) const
	{
		return lhs->get_depth() <= rhs->get_depth();
	}
};
