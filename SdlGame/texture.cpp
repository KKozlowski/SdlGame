#include "texture.h"
#include <SDL_image.h>
#include "engine.h"
#include "vector2.h"

std::unique_ptr<texture> texture::m_instance(nullptr);

texture::texture()
{
	m_loaded = new std::map<std::string, SDL_Texture*>();
}

texture::~texture()
{
	for (auto kvp : *m_loaded)
	{
		SDL_DestroyTexture(kvp.second);
	}

	m_loaded->clear();
	delete m_loaded;
}

SDL_Texture* texture::load_texture(std::string path)
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
	if (m_loaded->find(filename) == m_loaded->end())
	{
		SDL_Texture *t = load_texture(filename);
		if (t != nullptr)
			m_loaded->insert_or_assign(filename, t);
		return t;
	}
	else
		return m_loaded->at(filename);
}

SDL_Texture* texture::get_texture(std::string filename)
{
	if (m_instance.get() == nullptr)
		m_instance = std::unique_ptr<texture>(new texture());

	return m_instance.get()->get(filename);
}
