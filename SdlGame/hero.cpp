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

void hero::set_direction(point dir, bool with_jump = false)
{
	if (with_jump)
	{
		set_current_tile(destination_tile);
		movement_progress -= 1;
	} 
	destination_tile = current_tile->get_neighbor(dir);
}

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

void hero::set_current_tile(tile* t)
{
	if (t->get_gold() != nullptr)
	{
		add_points(t->get_gold()->get_value());
		t->pop_gold();

		std::cout << "HERO TAKES GOLD\n";
		m_levelgrid->on_hero_gold_take(m_points);
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
		

	current_tile = t;
}

void hero::continue_movement()
{
	movement_progress += engine::get_delta_time() * movement_speed;
	if (destination_tile == nullptr)
		get_transform()->position = current_tile->get_transform()->position;
	else
	{
		if (movement_progress >= 1)
		{
			set_current_tile(destination_tile);
			get_transform()->position = destination_tile->get_transform()->position;
			if (m_falling)
				movement_progress -= 1;
		}
		else
			get_transform()->position = tile::position_lerp(current_tile, destination_tile, movement_progress);
	}

	get_transform()->position -= position_offset;
}

bool hero::dig(point direction)
{
	tile *one = current_tile->get_neighbor(direction);
	if (one == nullptr || !(one->is_empty() || one->get_type() == tile_type::ladder)) 
		return false;

	tile *two = one->get_neighbor({ 0,1 });
	if (two == nullptr || two->get_type() != tile_type::wall) 
		return false;
	static_cast<wall *>(two)->dig();
}

hero::hero(tile *start_tile, level_grid *lg)
{
	draw_texture *dt = new draw_texture(this, "texture.png");
	dt->centered = true;
	m_draw = dt;

	movement_speed = 4.f;
	adjustment_jump_tolerance = 0.33f;

	set_current_tile(start_tile);
	m_levelgrid = lg;
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
	if (destination_tile == nullptr) return false;

	vector2f position = get_transform()->position;
	vector2f origin_position = current_tile->get_transform()->position;
	vector2f destiny_position = destination_tile->get_transform()->position;

	return (destiny_position - position).length() < m_levelgrid->get_tilesize()* adjustment_jump_tolerance;
}

void hero::die()
{
	m_alive = false;
	m_draw->set_visible(false);
	std::cout << "DEATH";
}

tile* hero::get_current_tile() const
{
	return current_tile;
}

void hero::update()
{
	reduce_offset();
	point zero;
	vector2f position = get_transform()->position;
	vector2f current_tile_position = current_tile->get_transform()->position;
	engine::get_instance()->get_renderer()->get_camera()->center = point(get_transform()->position.x, get_transform()->position.y);

	input *inp = engine::get_instance()->get_input();

	point dir;
	tile *old_destination = destination_tile;

	if (m_falling)
	{
		dir = { 0,1 };
		destination_tile = current_tile->get_down();
	}
	else if (is_alive())
	{
		if (inp->get_key(SDLK_q))
			dig({ -1,0 });
		else if (inp->get_key(SDLK_e))
			dig({ 1,0 });

		if (inp->get_key(SDLK_w))
		{
			if (can_jump_to_destination() && !destination_tile->empty_over_empty() && destination_tile->can_up())
			{
				set_direction(dir = { 0,-1 }, true);
			} 
			else if (current_tile->can_up() || position.y > current_tile_position.y)
			{
				set_direction(dir = { 0,-1 });
			} 
		}
		else if (inp->get_key(SDLK_s))
		{
			if (can_jump_to_destination() && !destination_tile->empty_over_empty() && destination_tile->can_down())
			{
				set_direction(dir = { 0,1 }, true);
			} 
			else if (current_tile->can_down() || position.y < current_tile_position.y)
			{
				set_direction(dir = { 0,1 });
				if (destination_tile->is_empty())
					m_falling = true;
			}
		}
		else if (inp->get_key(SDLK_a))
		{
			if (can_jump_to_destination() && !destination_tile->empty_over_empty() && destination_tile->can_left())
			{
				set_direction(dir = { -1,0 },true);
			} 
			else if (current_tile->can_left() || position.x > current_tile_position.x)
			{
				set_direction(dir = { -1,0 });
			}
		}
		else if (inp->get_key(SDLK_d) )
		{
			if (can_jump_to_destination() && !destination_tile->empty_over_empty() && destination_tile->can_left())
			{
				set_direction(dir = { 1,0 }, true);
			} 
			else if (current_tile->can_right() || position.x < current_tile_position.x)
			{
				set_direction(dir = { 1,0 });
			}
		}
	}

	if (dir == zero)
		return;

	if ((previous_dir * dir) == zero) { //movement direction changed completely.

		position_offset = current_tile->get_transform()->position - get_transform()->position;

		if (can_jump_to_destination()) 
		{
			set_direction(dir = { 0,0 }, true);
		}

		movement_progress = 0;
	}

	else if (previous_dir != dir && (previous_dir * dir) != zero) { //direction has changed, but not the axis.
		movement_progress = -movement_progress;
	} else //nothing changed
	{
		continue_movement();
	}

	previous_dir = dir;
}
