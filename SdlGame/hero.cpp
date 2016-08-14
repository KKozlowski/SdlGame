#include "hero.h"
#include "input.h"
#include "engine.h"
#include "camera.h"
#include "draw_texture.h"
#include "gold.h"
#include "tile.h"
#include "level_grid.h"
#include <iostream>

void hero::go_by_direction(point dir, bool with_jump = false)
{
	if (with_jump)
	{
		set_current_tile(destination_tile);
		movement_progress -= 1;
	}
	destination_tile = current_tile->get_neighbor(dir);
}

void hero::set_current_tile(tile* t)
{
	if (t->get_gold() != nullptr)
	{
		std::cout << "GOLD ENCOUNTERED\n";
		add_points(t->get_gold()->get_value());
		t->pop_gold();
	}

	if (t->empty_over_empty())
	{
		falling = true;
	}
		
	else if (t->over_solid())
		falling = false;

	current_tile = t;
}

hero::hero(tile *start_tile, level_grid *lg)
{
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

void hero::update()
{
	point zero;
	vector2f position = get_transform()->position;
	vector2f current_tile_position = current_tile->get_transform()->position;
	engine::get_instance()->get_renderer()->get_camera()->center = point(get_transform()->position.x, get_transform()->position.y);

	input *inp = engine::get_instance()->get_input();

	point dir;
	tile *old_destination = destination_tile;

	if (falling)
	{
		dir = { 0,1 };
		destination_tile = current_tile->get_down();
	}
	else
	{
		if (inp->get_key(SDLK_w))
		{
			if (can_jump_to_destination() && !destination_tile->empty_over_empty() && destination_tile->can_up())
			{
				go_by_direction(dir = { 0,-1 }, true);
			} 
			else if (current_tile->can_up() || position.y > current_tile_position.y)
			{
				go_by_direction(dir = { 0,-1 });
			} 
		}
		else if (inp->get_key(SDLK_s))
		{
			if (can_jump_to_destination() && !destination_tile->empty_over_empty() && destination_tile->can_down())
			{
				go_by_direction(dir = { 0,1 }, true);
			} 
			else if (current_tile->can_down() || position.y < current_tile_position.y)
			{
				go_by_direction(dir = { 0,1 });
				if (destination_tile->get_type() == tile_type::empty)
					falling = true;
			}
		}
		else if (inp->get_key(SDLK_a))
		{
			if (can_jump_to_destination() && !destination_tile->empty_over_empty() && destination_tile->can_left())
			{
				go_by_direction(dir = { -1,0 },true);
			} 
			else if (current_tile->can_left() || position.x > current_tile_position.x)
			{
				go_by_direction(dir = { -1,0 });
			}
		}
		else if (inp->get_key(SDLK_d) )
		{
			if (can_jump_to_destination() && !destination_tile->empty_over_empty() && destination_tile->can_left())
			{
				go_by_direction(dir = { 1,0 }, true);
			} 
			else if (current_tile->can_right() || position.x < current_tile_position.x)
			{
				go_by_direction(dir = { 1,0 });
			}
		}
	}
	
	
	if (dir == zero)
		return;

	if ((previous_dir * dir) == zero) { //movement direction changed completely.
		if (can_jump_to_destination()) 
		{
			get_transform()->position = destination_tile->get_transform()->position;
			set_current_tile(destination_tile);
		}
		else
			get_transform()->position = current_tile->get_transform()->position;

		movement_progress = 0;
	}

	else if (previous_dir != dir && (previous_dir * dir) != zero) { //direction has changed, but not the axis.
		movement_progress = -movement_progress;
	} else //nothing changed
	{
		movement_progress += engine::get_delta_time() * speed;
		if (destination_tile == nullptr)
			get_transform()->position = current_tile->get_transform()->position;
		else
		{
			if (movement_progress >=1)
			{
				set_current_tile(destination_tile);
				get_transform()->position = destination_tile->get_transform()->position;
				movement_progress -= 1;
			} else
				get_transform()->position = tile::position_lerp(current_tile, destination_tile, movement_progress);
		}
	}

	previous_dir = dir;
}
