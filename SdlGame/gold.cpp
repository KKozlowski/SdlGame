﻿#include "gold.h"
#include "level_grid.h"

gold::gold(int points)
{
	set_value(points);
	m_draw = new draw_texture(this, "gold.png");
	m_draw->set_depth(-20);
	static_cast<draw_texture*>(m_draw)->centered = true;
}

gold::~gold()
{
	m_levelgrid->on_gold_disappearance(this);
}

//
//gold::gold(tile* tile, int points)
//	: gold(points)
//{
//	bool success = set_tile(tile);
//	if (!success) delete this;
//}

bool gold::set_tile(tile* new_parent, level_grid *lg)
{
	m_levelgrid = lg;
	if (new_parent->get_gold() != nullptr) //If there's already a pile of gold on the tile, we just make it bigger.
	{
		new_parent->get_gold()->set_value(new_parent->get_gold()->get_value() + this->get_value());
		return false;
	} else //If the tile is goldless, we simply put gold on it.
	{
		parent = new_parent;
		parent->set_gold(this);
		get_transform()->position = parent->get_transform()->position;
		static_cast<draw_texture*>(m_draw)->set_width_height(lg->get_tilesize(), lg->get_tilesize());
		return true;
	}
}

int gold::get_value()
{
	return point_value;
}

void gold::set_value(int pts)
{
	point_value = pts;
}