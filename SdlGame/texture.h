#pragma once
#include <string>
#include <map>
#include <SDL.h>
#include <memory>

class texture
{
private:
	std::map<std::string, SDL_Texture*> *loaded;
	texture();
	static std::unique_ptr<texture> instance;
	SDL_Texture* loadTexture(std::string path);

public:
	SDL_Texture* get(std::string filename);
	static SDL_Texture* get_texture(std::string filename);
	~texture();
};
