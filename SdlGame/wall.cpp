﻿#include "wall.h"
#include <iostream>
#include "engine.h"

float wall::rebuilding_time = 4.f;

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

bool wall::dig(float delay)
{
	if (destructible)
	{
		m_timeOfDig = engine::get_time_from_start() + delay;
		time_to_refill = rebuilding_time;
		return true;
	}
	return false;
}

bool wall::is_empty()
{
	return digged && enemy_in_the_hole == nullptr;
}

void wall::update()
{
	if (m_timeOfDig != 0 && m_timeOfDig < engine::get_time_from_start())
	{
		get_draw()->set_visible(false);
		digged = true;
		m_timeOfDig = 0;
	}
	
	if (digged)
	{
		time_to_refill -= engine::get_delta_time();
		if (time_to_refill < 0)
			refill();
	}
}
