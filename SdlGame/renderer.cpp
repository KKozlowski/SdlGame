#include "renderer.h"
#include "camera.h"
#include <vector>
#include <algorithm>
#include <SDL_image.h>

bool renderer::sdl_initialized = false;

bool renderer::initialize_sdl()
{
	if (!sdl_initialized)
	{
		bool success = false;
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			success = false;
		}

		TTF_Init();
		return success;
	}
	else
		return true;
	
}

void renderer::close_sdl()
{
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

renderer::renderer()
{
	initialize_sdl();

	m_camera = new camera(point(640,360), SCREEN_HEIGHT*1.6f , this);

	mainWindow = SDL_CreateWindow("Lode Runner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (mainWindow == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		{
			screenSurface = SDL_GetWindowSurface(mainWindow);
		}

		mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
		if (mainRenderer == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);
		}
	}

	drawed = new std::set<draw_base *>();
}

renderer::~renderer()
{
	SDL_DestroyWindow(mainWindow);
	mainWindow = nullptr;

	SDL_DestroyRenderer(mainRenderer);
	mainRenderer = nullptr;
}

SDL_Renderer* renderer::get_renderer() const
{
	return mainRenderer;
}

SDL_Window* renderer::get_window() const
{
	return mainWindow;
}

camera* renderer::get_camera() const
{
	return m_camera;
}

void renderer::print_text(std::string content, point position, int height)
{
	if (font == nullptr)
		font = TTF_OpenFont("PrStart.ttf", 24); 

	SDL_Color White = { 255, 255, 255 }; 

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, content.c_str(), White); 

	SDL_Texture* Message = SDL_CreateTextureFromSurface(mainRenderer, surfaceMessage); 

	SDL_Rect Message_rect; 
	Message_rect.x = position.x; 
	Message_rect.y = position.y; 
	Message_rect.w = height*content.size(); 
	Message_rect.h = height; 

						  
	SDL_RenderCopy(mainRenderer, Message, NULL, &Message_rect);

	SDL_DestroyTexture(Message);
	SDL_FreeSurface(surfaceMessage);
	
}

void renderer::clear()
{
	clear(255, 255, 255);
}

void renderer::clear(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(mainRenderer, r, g, b, 255);
	SDL_RenderClear(mainRenderer);
}

void renderer::set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(mainRenderer, r, g, b, a);
}

bool renderer::add_draw(draw_base* d)
{
	drawed->insert(d);
	return true;
}

bool renderer::remove_draw(draw_base* d)
{
	drawed->erase(d);
	return true;
}

void renderer::draw()
{
	clear(0,0,0);

	std::vector<draw_base *> *to_draw = new std::vector<draw_base*>(drawed->begin(), drawed->end());

	struct {
		bool operator()(draw_base * a, draw_base * b)
		{
			return a->get_depth() > b->get_depth();
		}
	} depthComparator;

	sort(to_draw->begin(), to_draw->end(), depthComparator);

	for (draw_base *d : *to_draw)
		if (d->is_visible())
			d->draw(mainRenderer, m_camera);

	delete to_draw;
	if (bottom_text != "")
		print_text(bottom_text, point(10,SCREEN_HEIGHT-50), 50);

	SDL_RenderPresent(mainRenderer);
}
