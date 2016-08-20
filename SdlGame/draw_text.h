#pragma once
#include "draw_base.h"

class text_render;
class renderer;

class draw_text : public draw_base
{
private:
	renderer *m_renderer;
	text_render *text_data;
public:
	draw_text(text_render *a, renderer *r);
	void draw(SDL_Renderer* renderer, camera* cam) override;
};
