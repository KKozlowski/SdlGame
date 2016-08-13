#include "hero.h"
#include "input.h"
#include "engine.h"
#include "camera.h"
#include "draw_texture.h"

#include "tile.h"
#include "level_grid.h"
#include <iostream>

void hero::set_current_tile(tile* t)
{
	
	if (t->empty_over_empty())
		falling = true;
	else if (t->over_solid())
		falling = false;

	current_tile = t;
}

hero::hero(tile *start_tile, level_grid *lg)
{
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
	if (destination_tile == nullptr) return false;

	vector2f position = get_transform()->position;
	vector2f origin_position = current_tile->get_transform()->position;
	vector2f destiny_position = destination_tile->get_transform()->position;

	return (destiny_position - position).length() < (origin_position - position).length();
}

void hero::update()
{
	vector2f zero;
	vector2f position = get_transform()->position;
	vector2f current_tile_position = current_tile->get_transform()->position;
	engine::get_instance()->get_renderer()->get_camera()->center = point(get_transform()->position.x, get_transform()->position.y);

	input *inp = engine::get_instance()->get_input();

	vector2f dir;
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
			if (current_tile->can_up() || position.y > current_tile_position.y)
			{
				dir = { 0,-1 };
				destination_tile = current_tile->get_up();
			}
		}
		else if (inp->get_key(SDLK_s))
		{
			if (current_tile->can_down() || position.y < current_tile_position.y)
			{
				dir = { 0,1 };
				destination_tile = current_tile->get_down();
				if (destination_tile->get_type() == tile_type::empty)
					falling = true;
			}
		}
		else if (inp->get_key(SDLK_a))
		{

			if (current_tile->can_left() || position.x > current_tile_position.x)
			{
				dir = { -1,0 };
				destination_tile = current_tile->get_left();
			}
		}
		else if (inp->get_key(SDLK_d) )
		{
			if (current_tile->can_right() || position.x < current_tile_position.x)
			{
				dir = { 1,0 };
				destination_tile = current_tile->get_right();
			}
		}
	}
	
	
	if (dir == zero)
		return;

	if ((previous_dir * dir) == zero) { //movement direction changed completely.
		if (can_jump_to_destination()) {
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

				//destination_tile = nullptr;
			} else
				get_transform()->position = tile::position_lerp(current_tile, destination_tile, movement_progress);
		}
	}

	previous_dir = dir;
}
