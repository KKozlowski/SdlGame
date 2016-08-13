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

tile* tile::get_up()
{
	return m_level->get(indices.x, indices.y - 1);
}

tile* tile::get_down()
{
	return m_level->get(indices.x, indices.y + 1);
}

tile* tile::get_left()
{
	return m_level->get(indices.x - 1, indices.y);
}

tile* tile::get_right()
{
	return m_level->get(indices.x + 1, indices.y);
}

bool tile::can_up()
{
	return false;
}

bool tile::can_down()
{
	tile *down = get_down();
	//std::cout << (down == nullptr);
	if (!down) return false;
	if (down->get_type() != tile_type::wall) return true;
	
	return false;
}

bool tile::can_left()
{
	tile *t = get_left();

	if (!t) return false;
	if (t->get_type() != tile_type::wall) return true;

	return false;
}

bool tile::can_right()
{
	tile *t = get_right();

	if (!t) return false;
	if (t->get_type() != tile_type::wall) return true;

	return false;
}

bool tile::over_empty()
{
	return get_down() != nullptr && get_down()->get_type() == tile_type::empty;
}

bool tile::empty_over_empty()
{
	return m_type == tile_type::empty && get_down() != nullptr && get_down()->get_type() == tile_type::empty;
}

bool tile::over_solid()
{
	return get_down() == nullptr || get_down()->get_type() == tile_type::wall || get_down()->get_type() == tile_type::ladder;
}

vector2f tile::position_lerp(tile* start, tile* end, float percent)
{
	vector2f diff = end->get_transform()->position - start->get_transform()->position;
	return start->get_transform()->position + diff*percent;
}

