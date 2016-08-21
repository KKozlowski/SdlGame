#include "draw_text.h"
#include "renderer.h"
#include "text_render.h"
#include <iostream>

draw_text::draw_text(text_render *a, renderer* r)
	: draw_base(a), m_renderer(r), text_data(a)
{
	set_depth(-1000);
}

void draw_text::draw(SDL_Renderer* renderer, camera* cam)
{
	if (text_data->content != "")
		m_renderer->print_text(text_data->content, text_data->position, text_data->height);
}
