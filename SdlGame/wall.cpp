#include "wall.h"
#include <iostream>
#include "engine.h"

void wall::refill()
{
	get_draw()->set_visible(true);
	digged = false;
	if (get_level()->get_hero()->get_current_tile() == this)
		get_level()->get_hero()->die();
}

wall::wall(int x, int y, level_grid* l, bool destructible)
	:tile(x, y, l)
{
	digged = false;
	m_type = tile_type::wall;

	this->destructible = destructible;
	if (destructible)
		get_tex_draw()->set_texture("wall.png");
	else 
		get_tex_draw()->set_texture("super_wall.png");
}

void wall::dig()
{
	if (destructible)
	{
		get_draw()->set_visible(false);
		digged = true;
		time_to_refill = 5.f;
	}
}

bool wall::is_empty()
{
	return digged;
}

void wall::update()
{
	if (digged)
	{
		time_to_refill -= engine::get_delta_time();
		if (time_to_refill < 0)
			refill();
	}
}
