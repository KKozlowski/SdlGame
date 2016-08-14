#include "wall.h"
#include <iostream>
#include "engine.h"

void wall::refill()
{
	get_draw()->set_visible(true);
	digged = false;
}

wall::wall(int x, int y, level_grid* l)
	:tile(x, y, l)
{
	digged = false;
	m_type = tile_type::wall;
	get_tex_draw()->set_texture("wall.png");
}

void wall::dig()
{
	get_draw()->set_visible(false);
	digged = true;
	time_to_refill = 5.f;
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
