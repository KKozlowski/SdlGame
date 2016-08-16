#pragma once
#include <SDL.h>
#include <string>
#include "renderer.h"
#include "input.h"
#include <ctime>
#include "level_grid.h"

class scene;
class level_manager;

class engine
{
private: 
	bool quit = false;
	float delta_time = 0.00000001f;

	engine();

	renderer *m_renderer;
	input *m_input;
	scene *m_actormanager;

	level_manager *m_levelmanager;
public:
	~engine();
	static engine* get_instance();
	static scene* get_scene();

	renderer *get_renderer() const;
	input* get_input() const;
	
	///<summary>
	///Returns time (in seconds) that passed from start to end of the previous frame.
	///<para/>It is accurate to nanoseconds.
	///</summary>
	static float get_delta_time();

	void run();
};
