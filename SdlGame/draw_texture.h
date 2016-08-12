#pragma once
#include "draw_base.h"
#include <string>
#include "texture.h"


class draw_texture : public draw_base
{
private:
	SDL_Texture *thatTexture;
	SDL_Rect stretchRect;
public:
	bool centered;
	explicit draw_texture(actor* a, std::string filename);
	void draw(SDL_Renderer *renderer, camera *cam) override;
};
