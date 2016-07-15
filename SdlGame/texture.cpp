#include "texture.h"
#include <SDL_image.h>
#include "engine.h"
#include "vector2.h"

std::unique_ptr<texture> texture::instance;

texture::texture()
{
	loaded = new std::map<std::string, SDL_Texture*>();
}

texture::~texture()
{
	for (auto kvp : *loaded)
	{
		SDL_DestroyTexture(kvp.second);
	}

	loaded->clear();
	delete loaded;
}

SDL_Texture* texture::loadTexture(std::string path)
{
	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(engine::get_instance()->get_renderer()->get_renderer(), loadedSurface);
		if (newTexture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

SDL_Texture* texture::get(std::string filename)
{
	if (loaded->find("filename") == loaded->end())
	{
		SDL_Texture *t = loadTexture(filename);
		if (t != nullptr)
			loaded->insert_or_assign(filename, t);
		return t;
	}
	else
		return loaded->at(filename);
}

SDL_Texture* texture::get_texture(std::string filename)
{
	if (instance.get() == nullptr)
		instance.reset(new texture());
	return instance.get()->get(filename);
}
