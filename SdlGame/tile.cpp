#include "tile.h"
#include "level_grid.h"
#include <iostream>

tile::tile(int x, int y, level_grid* l)
{
	indices = point(x, y);
	m_level = l;
	drawing = new draw_texture(this, "");
	get_tex_draw()->centered = true;
	get_tex_draw()->set_depth(10);
}

bool tile::can_up()
{
	return false;
}

bool tile::can_down()
{
	tile *down = m_level->get(indices.x, indices.y + 1);
	//std::cout << (down == nullptr);
	if (!down) return false;
	if (down->get_type() != tile_type::wall) return true;
	
	return false;
}

bool tile::can_left()
{
	tile *t = m_level->get(indices.x-1, indices.y);

	if (!t) return false;
	if (t->get_type() != tile_type::wall) return true;

	return false;
}

bool tile::can_right()
{
	tile *t = m_level->get(indices.x+1, indices.y);

	if (!t) return false;
	if (t->get_type() != tile_type::wall) return true;

	return false;
}


