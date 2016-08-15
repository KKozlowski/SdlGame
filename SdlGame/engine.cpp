#include "engine.h"
#include <cstdio>
#include <SDL_image.h>
#include "texture.h"
#include <chrono>
#include "ball.h"
#include "scene.h"
#include <iostream>
#include "hero.h"
#include "level_grid.h"
#include "level_manager.h"

bool engine::initialize()
{
	bool success = true;

	thatTexture = texture::get_texture("texture.png");
	if (thatTexture == nullptr)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	return success;
}

engine::engine()
{
	render = new renderer();
	inputer = new input();
	actor_manager = new scene();
	m_levelman = new level_manager();
}

engine::~engine()
{
	delete inputer;
	inputer = nullptr;

	delete render;
	render == nullptr;

	delete m_levelman;
	m_levelman == nullptr;
	renderer::close_sdl();
}

engine* engine::get_instance()
{
	static engine instance;
	return &instance;
}

scene* engine::get_scene()
{
	return get_instance()->actor_manager;
}

renderer* engine::get_renderer() const
{
	return render;
}

input* engine::get_input() const
{
	return inputer;
}

float engine::get_delta_time()
{
	return get_instance()->delta_time;
}

void engine::run()
{
	if (!initialize())
	{
		printf("Failed to initialize!\n");
		return;
	}

	m_levelman->load_level(0, 160);

	SDL_Event e;
	
	time_since_epoch = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	uint64_t time_since_epoch_temp = time_since_epoch;

	const double num = std::chrono::high_resolution_clock::period::num;
	const double den = std::chrono::high_resolution_clock::period::den;
	
	//MAIN LOOP
	while (!quit)
	{
		inputer->next_frame();
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				printf("MMMM");
				quit = true;
			}
			else
			{
				inputer->handle_event(e);
			}
		}

		actor_manager->update();
		render->draw();

		// DO ADDITIONAL STUFF BOLOW THIS LINE
		//////////////////////

		if (!m_levelman->get_current()->get_hero()->is_alive())
			m_levelman->reset_level();
		if (inputer->get_key(SDLK_ESCAPE)) quit = true;

		//////////////////////
		// AND ABOVE THIS LINE

		// Calculating delta time.
		time_since_epoch_temp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		delta_time = (time_since_epoch_temp - time_since_epoch) * num / den;
		time_since_epoch = time_since_epoch_temp;
	}
}
