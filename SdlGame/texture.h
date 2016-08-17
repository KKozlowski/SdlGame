#pragma once
#include <string>
#include <map>
#include <SDL.h>
#include <memory>
///<summary>
/// A singleton class that holds all the loaded textures.
///</summary>
class texture
{
private:
	std::map<std::string, SDL_Texture*> *m_loaded;
	static std::unique_ptr<texture> m_instance;
	texture();
	SDL_Texture* load_texture(std::string path);
	SDL_Texture* get(std::string filename);

public:
	///<summary>Loads and returns (or, if it's already loaded just returns) a texture from a file of given name</summary>
	static SDL_Texture* get_texture(std::string filename);
	~texture();
};
