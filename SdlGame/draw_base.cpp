﻿#include "draw_base.h"
#include "engine.h"
#include "renderer.h"

draw_base::draw_base(actor* a)
{
	m_actor = a;
	engine::get_instance()->get_renderer()->add_draw(this);
	set_visible(true);
}

draw_base::~draw_base()
{
	engine::get_instance()->get_renderer()->remove_draw(this);
}



void draw_base::draw(SDL_Renderer* renderer, camera* cam)
{
}
