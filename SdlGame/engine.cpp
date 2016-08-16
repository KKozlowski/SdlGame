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
	m_levelmanager = new level_manager();
}

engine::~engine()
{
	delete m_input;
	m_input = nullptr;

	delete m_levelmanager;
	m_levelmanager == nullptr;

	renderer::close_sdl();
	delete m_renderer;
	m_renderer == nullptr;
	
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

void engine::run()
{
	m_levelmanager->load_level(1, 160);

	SDL_Event e;
	
	uint64_t time_since_epoch = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	uint64_t time_since_epoch_temp = time_since_epoch;

	const double num = std::chrono::high_resolution_clock::period::num;
	const double den = std::chrono::high_resolution_clock::period::den;
	
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
		hero *he = nullptr;
		if (m_levelmanager->get_current())
			he = m_levelmanager->get_current()->get_hero();
		if (he != nullptr)
		{
			if (!he->is_alive())
				m_levelmanager->reset_level();
			else if (he->has_won())
			{
				if (!m_levelmanager->load_next_level())
					quit = true;
			}
		}
		
		if (m_input->get_key(SDLK_ESCAPE)) quit = true;
		
		//////////////////////
		// AND ABOVE THIS LINE

		// Calculating delta time.
		time_since_epoch_temp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		delta_time = (time_since_epoch_temp - time_since_epoch) * num / den;
		time_since_epoch = time_since_epoch_temp;
	}
}
