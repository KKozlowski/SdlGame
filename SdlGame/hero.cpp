#include "hero.h"
#include "input.h"
#include "engine.h"
#include "camera.h"
#include "draw_texture.h"
#include "gold.h"
#include "tile.h"
#include "level_grid.h"
#include "level_manager.h"
#include <iostream>

void hero::reduce_offset()
{
	if (position_offset == vector2f())
		return;

	float speed = m_levelgrid->get_tilesize() * offset_reduction_speed;

	vector2f old_offset = position_offset;

	if (position_offset.x != 0)
		position_offset.x -= (position_offset.x / abs(position_offset.x))*speed*engine::get_delta_time();
	if (position_offset.y != 0)
		position_offset.y -= (position_offset.y / abs(position_offset.y))*speed*engine::get_delta_time();

	if (old_offset.x*position_offset.x < 0) position_offset.x = 0;
	if (old_offset.y*position_offset.y < 0) position_offset.y = 0;

	get_transform()->position += (old_offset - position_offset);
}

void hero::set_direction(point dir, bool with_jump = false)
{
	if (with_jump)
	{
		set_current_tile(m_destinationTile);
		m_movementProgress -= 1;
	}
	set_destination_tile(m_currentTile->get_neighbor(dir));
}

void hero::set_current_tile(tile* t)
{
	if (t->get_gold() != nullptr)
	{
		point new_indices = t->get_indices();

		add_golden_points(t->get_gold()->get_value());
		t->pop_gold();

		std::cout << "HERO TAKES GOLD\n";
		m_levelgrid->on_hero_gold_take(m_goldenPoints, m_killerPoints);

		t = m_levelgrid->get(new_indices.x, new_indices.y);
	}

	
	if (t->ends_the_level())
	{
		m_winning = true;
	}

	if (t->empty_over_empty())
	{
		m_falling = true;
	}
		
	else if (m_falling && (t->over_solid() || t->get_type() == tile_type::pipe))
	{
		m_falling = false;
	}
		
	tile_traveller::set_current_tile(t);
}

void hero::continue_movement()
{
	m_movementProgress += engine::get_delta_time() * m_movementSpeed;

	if (m_destinationTile == nullptr)
		get_transform()->position = m_currentTile->get_transform()->position;
	else
	{
		if (m_movementProgress >= 1.f)
		{
			set_current_tile(m_destinationTile);
			get_transform()->position = m_destinationTile->get_transform()->position;
			m_movementProgress -=1.f;
		}
		else
			get_transform()->position = tile::position_lerp(m_currentTile, m_destinationTile, m_movementProgress);
	}

	get_transform()->position -= position_offset;
}

bool hero::dig(point direction)
{
	tile *one = m_currentTile->get_neighbor(direction);
	if (one == nullptr || !(one->is_empty() || one->get_type() == tile_type::ladder || one->get_type() == tile_type::pipe))
		return false;

	tile *two = one->get_neighbor({ 0,1 });
	if (two == nullptr || two->get_type() != tile_type::wall) 
		return false;

	bool success = static_cast<wall *>(two)->dig(m_diggingTime);
	if (success)
	{
		m_timeOfDiggingStop = engine::get_time_from_start() + 0.25f;
		static_cast<draw_texture *>(get_draw())->set_texture("texture_shovel.png");
	}

	return success;
}

hero::hero(tile *start_tile, level_grid *lg)
	: tile_traveller(lg, 4.f)
{
	draw_texture *dt = new draw_texture(this, "texture.png");
	dt->centered = true;
	m_draw = dt;

	adjustment_jump_tolerance = 0.33f;

	set_current_tile(start_tile);
	float tilesize = lg->get_tilesize();

	static_cast<draw_texture *>(get_draw())->set_width_height(tilesize, tilesize);

	get_transform()->position = vector2f(start_tile->get_Xpos()*tilesize, start_tile->get_Ypos()*tilesize);

	get_draw()->set_depth(-10);
}

hero::~hero()
{

}

bool hero::can_jump_to_destination() const
{
	if (m_destinationTile == nullptr) return false;

	vector2f position = get_transform()->position;
	vector2f origin_position = m_currentTile->get_transform()->position;
	vector2f destiny_position = m_destinationTile->get_transform()->position;

	return (destiny_position - position).length() < m_levelgrid->get_tilesize()* adjustment_jump_tolerance;
}

void hero::stop_digging()
{
	m_timeOfDiggingStop = 0;
	static_cast<draw_texture *>(get_draw())->set_texture("texture.png");
}

void hero::die()
{
	m_alive = false;
	m_draw->set_visible(false);
	std::cout << "DEATH";
}

tile* hero::get_current_tile() const
{
	return m_currentTile;
}

void hero::update()
{
	reduce_offset();
	point zero;
	
	engine::get_instance()->get_renderer()->get_camera()->center = point(get_transform()->position.x, get_transform()->position.y);

	point dir;
	tile *old_destination = m_destinationTile;

	if (is_digging() && m_timeOfDiggingStop < engine::get_time_from_start())
		stop_digging();

	if (m_falling)
	{
		dir = { 0,1 };
		set_destination_tile(m_currentTile->get_down());
	}
	else if (is_alive() && !is_digging())
	{
		dir = read_and_apply_input();
	}

	if (dir == zero)
		return;

	if ((m_previousDirection * dir) == zero) { //movement direction changed completely.

		position_offset = m_currentTile->get_transform()->position - get_transform()->position;

		if (can_jump_to_destination()) 
		{
			set_direction(dir = { 0,0 }, true);
		}

		m_movementProgress = 0;
	}

	else if (m_previousDirection != dir && (m_previousDirection * dir) != zero) { //direction has changed, but not the axis.
			m_movementProgress = -m_movementProgress;
	} else //nothing changed
	{
		continue_movement();
	}

	m_previousDirection = dir;
}

point hero::read_and_apply_input()
{
	input *inp = engine::get_instance()->get_input();

	vector2f position = get_transform()->position;
	vector2f current_tile_position = m_currentTile->get_transform()->position;

	point result;

	if (inp->get_key(SDLK_q))
		dig({ -1,0 });
	else if (inp->get_key(SDLK_e))
		dig({ 1,0 });
	else if (inp->get_key(SDLK_w))
	{
		if (can_jump_to_destination() && !m_destinationTile->empty_over_empty() && m_destinationTile->can_up())
		{
			set_direction(result = { 0,-1 }, true);
		}
		else if (m_currentTile->can_up() || position.y > current_tile_position.y)
		{
			set_direction(result = { 0,-1 });
		}
	}
	else if (inp->get_key(SDLK_s))
	{
		if (can_jump_to_destination() && !m_destinationTile->empty_over_empty() && m_destinationTile->can_down())
		{
			set_direction(result = { 0,1 }, true);
		}
		else if (m_currentTile->can_down() || position.y < current_tile_position.y)
		{
			set_direction(result = { 0,1 });
			if (m_destinationTile->is_empty())
				m_falling = true;
		}
	}
	else if (inp->get_key(SDLK_a))
	{
		if (can_jump_to_destination() && !m_destinationTile->empty_over_empty() && m_destinationTile->can_left())
		{
			set_direction(result = { -1,0 }, true);
		}
		else if (m_currentTile->can_left() || position.x > current_tile_position.x)
		{
			set_direction(result = { -1,0 });
		}
	}
	else if (inp->get_key(SDLK_d))
	{
		if (can_jump_to_destination() && !m_destinationTile->empty_over_empty() && m_destinationTile->can_left())
		{
			set_direction(result = { 1,0 }, true);
		}
		else if (m_currentTile->can_right() || position.x < current_tile_position.x)
		{
			set_direction(result = { 1,0 });
		}
	}

	return result;
}
