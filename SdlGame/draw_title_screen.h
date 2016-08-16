#pragma once
#include "draw_base.h"

class draw_title_screen : public draw_base
{
public:
	draw_title_screen(actor *a);
	void draw(SDL_Renderer *renderer, camera *cam) override;
};
