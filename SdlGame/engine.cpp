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
	sceneManager = new scene();
}

engine::~engine()
{
	delete inputer;
	inputer = nullptr;

	delete render;
	render == nullptr;

	delete level;
	level == nullptr;
	renderer::close_sdl();
}

engine* engine::get_instance()
{
	static engine instance;
	return &instance;
}

scene* engine::get_scene()
{
	return get_instance()->sceneManager;
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

	level = new level_grid("level_one.txt", 160, point());

	SDL_Event e;

	ball *b = new ball();
	get_scene()->add_actor(b);
	
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

		sceneManager->update();
		render->draw();

		// DO ADDITIONAL STUFF BOLOW THIS LINE
		//////////////////////

		if (inputer->get_key(SDLK_UP)) printf("UP\n");
		if (inputer->get_key(SDLK_ESCAPE)) quit = true;

		auto mainRenderer = render->get_renderer();

		render->set_color(255, 0, 0, 255);
		SDL_RenderDrawPoint(mainRenderer, 100, 100);
		SDL_RenderDrawPoint(mainRenderer, 101, 100);
		SDL_RenderDrawPoint(mainRenderer, 102, 100);

		//////////////////////
		// AND ABOVE THIS LINE

		// Calculating delta time.
		time_since_epoch_temp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		delta_time = (time_since_epoch_temp - time_since_epoch) * num / den;
		time_since_epoch = time_since_epoch_temp;
	}
	get_scene()->remove_actors({ b });

	delete b;
}
