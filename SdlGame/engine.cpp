#include "engine.h"
#include <cstdio>
#include <SDL_image.h>
#include "texture.h"
#include <chrono>
#include "scene.h"
#include <iostream>
#include "hero.h"
#include "level_grid.h"
#include "level_manager.h"

engine::engine()
{
	m_renderer = new renderer();
	m_input = new input();

	m_actormanager = new scene();
	
}

engine::~engine()
{
	delete m_input;
	m_input = nullptr;

	delete m_renderer;
	m_renderer == nullptr;
	renderer::close_sdl();
}

engine* engine::get_instance()
{
	static engine instance;
	return &instance;
}

scene* engine::get_scene()
{
	return get_instance()->m_actormanager;
}

renderer* engine::get_renderer() const
{
	return m_renderer;
}

input* engine::get_input() const
{
	return m_input;
}

float engine::get_delta_time()
{
	return get_instance()->delta_time;
}

float engine::get_time_from_start()
{
	return get_instance()->time_from_start;
}

void engine::run()
{
	SDL_Event e;
	
	uint64_t time_since_epoch = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	uint64_t time_since_epoch_temp = time_since_epoch;

	const double num = std::chrono::high_resolution_clock::period::num;
	const double den = std::chrono::high_resolution_clock::period::den;
	
	level_manager *levels = new level_manager(160);
	m_actormanager->add_actor(levels);

	//MAIN LOOP
	while (!quit)
	{
		m_input->next_frame();
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else
			{
				m_input->handle_event(e);
			}
		}

		m_actormanager->update();
		m_renderer->draw();

		// DO ADDITIONAL STUFF BOLOW THIS LINE
		//////////////////////
		
		if (levels->is_finished()) quit = true;
		if (m_input->get_key(SDLK_ESCAPE)) quit = true;
		
		//////////////////////
		// AND ABOVE THIS LINE

		// Calculating delta time.
		time_since_epoch_temp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		delta_time = (time_since_epoch_temp - time_since_epoch) * num / den;
		time_from_start += delta_time;
		time_since_epoch = time_since_epoch_temp;
	}
}
