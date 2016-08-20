#include "text_render.h"
#include "draw_text.h"
#include "engine.h"

text_render::text_render()
{
	m_draw = new draw_text(this, engine::get_instance()->get_renderer());
}

text_render::text_render(std::string text, point start_point, int size)
	: text_render()
{
	content = text; 
	position = start_point; 
	height = size;
}
