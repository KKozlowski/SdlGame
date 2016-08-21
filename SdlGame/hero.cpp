#include "hero.h"
#include "input.h"
#include "engine.h"
#include "camera.h"
#include "draw_texture.h"
#include "gold.h"
#include "tile.h"
#include "level_grid.h"
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
	handle_direction_change(dir);

	direction_has_changed_this_frame = true;
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
			if (m_previousDirection.x != 0 || m_falling)
				m_movementProgress -=1.f;
		}
		else
			get_transform()->position = tile::position_lerp(m_currentTile, m_destinationTile, m_movementProgress);
	}

	get_transform()->position -= position_offset;
}

bool hero::dig(point direction)
{
	if (!can_take_input())
		return false;

	tile *considered_tile = get_current_tile();

	if (can_jump_to_destination())
		considered_tile = m_destinationTile;

	tile *one = considered_tile->get_neighbor(direction);
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

		set_current_tile(considered_tile);
		position_offset = m_currentTile->get_transform()->position - get_transform()->position;
		m_movementProgress = 0;
	}

	return success;
}

point hero::move_side(int side)
{
	point result;

	if (can_take_input())
	{
		vector2f position = get_transform()->position;
		vector2f current_tile_position = m_currentTile->get_transform()->position;

		point wanted_reult = { side,0 };

		bool prepared_for_adjustment_jump = can_jump_to_destination() && !m_destinationTile->empty_over_empty();
		bool prepared_for_standard_movement = m_currentTile->can(wanted_reult);
		if (side < 0)
			prepared_for_standard_movement = prepared_for_standard_movement || position.x > current_tile_position.x;
		else if (side > 0)
			prepared_for_standard_movement = prepared_for_standard_movement || position.x < current_tile_position.x;

		//WHAT CAN HAPPEN NOW? (condition order)
		// 1. hero just continues horizontal movement.
		if (prepared_for_standard_movement 
			&& m_previousDirection.y == 0)
		{
			set_direction(result = wanted_reult);
		}

		// 2. hero is not of floor, and he is very close to his destination tile.
		//    If he will move now, we will check if the destination tile allows his movement
		//    in given direction. If it does, his current tile will be overwritten with his destiantion tile.
		//    Offset will be set. "ADJUSTMENT JUMP" will be performed.
		else if (prepared_for_adjustment_jump
			&& m_destinationTile->can(wanted_reult))
		{
			set_direction(result = wanted_reult, true);
		}

		// 3. Hero is not on floor, and he cannot perform "ADJUSTMENT JUMP" to achieve his new direction.
		//    Hero moves in the most normal way.
		else if (prepared_for_standard_movement)
		{
			set_direction(result = wanted_reult);
		}
	}

	return result;
}

point hero::move_down()
{
	point result;

	if (can_take_input())
	{
		vector2f position = get_transform()->position;
		vector2f current_tile_position = m_currentTile->get_transform()->position;

		bool prepared_for_adjustment_jump = can_jump_to_destination() && !m_destinationTile->empty_over_empty();

		if (prepared_for_adjustment_jump && m_destinationTile->can_down())
		{
			set_direction(result = { 0,1 }, true);
		}
		else if (position.y < current_tile_position.y || m_currentTile->can_down())
		{
			set_direction(result = { 0,1 });

			if (m_destinationTile->is_empty())
			{
				if (m_currentTile->get_type() != tile_type::ladder    
					|| (m_currentTile->get_type() == tile_type::ladder  //Special case for falling down the last step of the ladder
						&& can_jump_to_destination()))
					m_falling = true;
			}
		}
	}
	return result;
}

point hero::move_up()
{
	point result;

	if (can_take_input())
	{
		vector2f position = get_transform()->position;
		vector2f current_tile_position = m_currentTile->get_transform()->position;

		bool prepared_for_adjustment_jump = can_jump_to_destination() && !m_destinationTile->empty_over_empty();

		if (prepared_for_adjustment_jump && m_destinationTile->can_up()) //Movement with distance skip, when near the tile that allows movement in given direction.
		{
			set_direction(result = { 0,-1 }, true);
		}
		else if (position.y > current_tile_position.y || m_currentTile->can_up()) //Standard movement to the next tile, or back to the current tile.
		{
			set_direction(result = { 0,-1 });
		}
	}
	return result;
}

point hero::move_idle()
{
	handle_direction_change({0,0});
	return { 0,0 };
}

void hero::handle_direction_change(point dir)
{
	if (direction_has_changed_this_frame)
		return;

	point zero;

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
	}
	else //nothing changed
	{
		m_previousDirection = dir;
		continue_movement();
	}

	m_previousDirection = dir;
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
	if (m_alive)
	{
		m_alive = false;
		m_draw->set_visible(false);
		std::cout << "DEATH\n";
	}
}

void hero::update()
{
	if (m_levelgrid == nullptr)
		return;

	if (get_current_tile()->is_death_trap())
		die();
	reduce_offset();
	
	engine::get_instance()->get_renderer()->get_camera()->center = point(get_transform()->position.x, get_transform()->position.y);

	point dir;
	tile *old_destination = m_destinationTile;

	if (is_digging() && m_timeOfDiggingStop < engine::get_time_from_start())
		stop_digging();

	if (m_falling)
	{
		set_direction({ 0,1 });
	}

	direction_has_changed_this_frame = false;
}
