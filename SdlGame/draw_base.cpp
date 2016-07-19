#include "draw_base.h"
#include "engine.h"
#include "renderer.h"

draw_base::draw_base(actor* a)
{
	lActor = a;
	engine::get_instance()->get_renderer()->add_draw(this);

}

draw_base::~draw_base()
{
	engine::get_instance()->get_renderer()->remove_draw(this);
}

int16_t draw_base::get_depth() const
{
	return layer;
}

void draw_base::set_depth(int16_t new_layer)
{
	layer = new_layer;
}

actor* draw_base::get_actor() const
{
	return lActor;
}

void draw_base::draw(SDL_Renderer *renderer)
{
}
