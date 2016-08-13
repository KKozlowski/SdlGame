#include "wall.h"
#include <iostream>

wall::wall(int x, int y, level_grid* l)
	:tile(x, y, l)
{
	m_type = tile_type::wall;
	get_tex_draw()->set_texture("wall.png");
}
