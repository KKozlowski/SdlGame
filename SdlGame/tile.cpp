#include "tile.h"
#include "level_grid.h"
#include <iostream>
#include "gold.h"
#include "engine.h"
#include "scene.h"

tile::tile(int x, int y, level_grid* l)
{
	m_indices = point(x, y);
	m_level = l;
	m_draw = new draw_texture(this, "");
	get_tex_draw()->centered = true;
	get_tex_draw()->set_depth(10);
}

tile* tile::get_up()
{
	return m_level->get(m_indices.x, m_indices.y - 1);
}

tile* tile::get_down()
{
	return m_level->get(m_indices.x, m_indices.y + 1);
}

tile* tile::get_left()
{
	return m_level->get(m_indices.x - 1, m_indices.y);
}

tile* tile::get_right()
{
	return m_level->get(m_indices.x + 1, m_indices.y);
}

tile* tile::get_neighbor(point offset)
{
	return m_level->get(m_indices.x + offset.x, m_indices.y + offset.y);
}

bool tile::can_up(bool for_ai)
{
	return false;
}

bool tile::can_down(bool for_ai)
{
	tile *down = get_down();
	if (!down) return false;
	if (down->get_type() != tile_type::wall) return true;
	if (!for_ai && down->is_empty()) return true;
	
	return false;
}

bool tile::can_left(bool for_ai)
{
	tile *t = get_left();

	if (!t) return false;
	if (!for_ai && t->is_empty()) return true;
	if (t->get_type() != tile_type::wall) return true;

	return false;
}

bool tile::can_right(bool for_ai)
{
	tile *t = get_right();

	if (!t) return false;
	if (!for_ai && t->is_empty()) return true;
	if (t->get_type() != tile_type::wall) return true;

	return false;
}

bool tile::over_empty()
{
	return get_down() != nullptr && get_down()->is_empty();
}

bool tile::empty_over_empty()
{
	return is_empty() && get_down() != nullptr && get_down()->is_empty();
}

bool tile::over_solid()
{
	return get_down() == nullptr || get_down()->get_type() == tile_type::wall || get_down()->get_type() == tile_type::ladder;
}

bool tile::set_gold(gold* g)
{
	if (m_gold == nullptr)
	{
		//std::cout << "PUTTING GOLD " << indices.to_string() << std::endl;
		m_gold = g;
		return true;
	}
	else
		return false;
}

gold* tile::get_gold() const
{
	return m_gold;
}

void tile::pop_gold()
{
	if (m_gold != nullptr)
	{
		//std::cout << "POPPING GOLD " << indices.to_string() << std::endl;
		engine::get_instance()->get_scene()->remove_actor(m_gold);
		delete m_gold;
		m_gold = nullptr;
	}
}

vector2f tile::position_lerp(tile* start, tile* end, float progress)
{
	return vector2f::lerp(start->get_transform()->position, end->get_transform()->position, progress);
}

