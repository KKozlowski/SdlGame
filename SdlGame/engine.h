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
	bool initialize();
	bool quit = false;
	float delta_time = 0.00000001f;

	uint64_t time_since_epoch;
	clock_t delta_clock;
	engine();

	renderer *render;
	input *inputer;
	scene *actor_manager;

	level_manager *m_levelman;
public:
	~engine();
	static engine* get_instance();
	static scene* get_scene();

	renderer *get_renderer() const;
	input* get_input() const;
	
	///<summary>
	///Returns time (in seconds) that passed from start to end of the previous frame.
	///</summary>
	static float get_delta_time();

	void run();
};
