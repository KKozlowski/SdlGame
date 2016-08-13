﻿#include "ladder.h"

ladder::ladder(int x, int y, level_grid* l)
	:tile(x,y,l)
{
	m_type = tile_type::ladder;
	get_tex_draw()->set_texture("ladder.png");
}

bool ladder::can_up()
{
	return get_up() != nullptr && get_up()->get_type() != tile_type::wall;
}
