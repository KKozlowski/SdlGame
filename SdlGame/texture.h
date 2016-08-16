#pragma once
#include <string>
#include <map>
#include <SDL.h>
#include <memory>

class texture
{
private:
	std::map<std::string, SDL_Texture*> *m_loaded;
	static std::unique_ptr<texture> m_instance;

	texture();
	SDL_Texture* load_texture(std::string path);

public:
	SDL_Texture* get(std::string filename);
	static SDL_Texture* get_texture(std::string filename);
	~texture();
};
