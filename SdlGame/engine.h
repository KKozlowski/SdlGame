#pragma once
#include <SDL.h>
#include <string>
#include "renderer.h"
#include "input.h"
#include <ctime>
#include "level_grid.h"

class scene;
class level_manager;
class ui;

class engine
{
private: 
	bool quit = false;
	float delta_time = 0.016f;
	float time_from_start = 0;

	engine();

	renderer *m_renderer;
	input *m_input;
	scene *m_actormanager;
	ui *m_ui;
public:
	~engine();
	static engine* get_instance();
	static scene* get_scene();

	ui *get_ui() const;
	renderer *get_renderer() const;
	input* get_input() const;
	
	///<summary>
	///Returns time (in seconds) that passed from start to end of the previous frame.
	///<para/>It is accurate to nanoseconds.
	///</summary>
	static float get_delta_time();

	///<summary>
	///Returns time (in seconds) that passed from start start of the game
	///<para/>It is accurate to nanoseconds.
	///</summary>
	static float get_time_from_start();

	///<summary>
	///Runs the main loop of the game. It returns when the game is over.
	///</summary>
	void run();
};
