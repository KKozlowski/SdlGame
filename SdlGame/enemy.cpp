#include "enemy.h"
#include "draw_texture.h"
#include "tile.h"
#include "level_grid.h"
#include "engine.h"

void enemy::set_current_tile(tile* t)
{
	if (t->can_down() && !t->can_down(true)) //TRAP!
	{
		falling_into_trap = true;
		destination_tile = t->get_down();
		wall *our_doom = static_cast<wall *>(destination_tile);
		if (our_doom != nullptr)
			our_doom->enemy_in_the_hole = this;
	}

	current_tile = t;
	movement_progress -= 1;
}

point enemy::get_2d_distance_to_tile(tile* t)
{
	return point(
		t->get_Xpos() - current_tile->get_Xpos(), 
		t->get_Ypos() - current_tile->get_Ypos()
		);
}

point enemy::find_move_to(tile* t)
{
	point result(0, 0);
	point where_to_go = get_2d_distance_to_tile(t);
	if (where_to_go.y == 0) //WE ARE ON THE SAME LEVEL!
	{
		tile *next_tile = current_tile;
		bool found = false;

		if (where_to_go.x < 0) //go left
		{
			//do we have clear path?
			do {
				if (next_tile->can_left(true))
				{
					next_tile = next_tile->get_left();
					if (next_tile == t)
						found = true;
				}
				else break;
			} while (!found);
			if (found)
				result = { -1,0 };
		} else if (where_to_go.x > 0)
		{
			do {
				if (next_tile->can_right(true))
				{
					next_tile = next_tile->get_right();
					if (next_tile == t)
						found = true;
				}
				else break;
			} while (!found);

			if (found)
				result = { 1,0 };
		}
	}
	//if (result != point()) std::cout << result.to_string() << std::endl;
	return result;
}

void enemy::die()
{
	drawing->set_visible(false);
}

enemy::enemy(tile* start_tile, level_grid* lg)
{
	m_levelgrid = lg;
	draw_texture *dt = new draw_texture(this, "enemy.png");
	dt->set_width_height(lg->get_tilesize(), lg->get_tilesize());
	dt->centered = true;
	drawing = dt;

	set_current_tile(start_tile);
	get_transform()->position = start_tile->get_transform()->position;

	m_speed = 2.f;
}

void enemy::update()
{
	hero *he = m_levelgrid->get_hero();
	if ((get_transform()->position - he->get_transform()->position).length() < killing_range*m_levelgrid->get_tilesize())
	{
		he->die();
	}

	movement_progress += m_speed * engine::get_delta_time();
	if (movement_progress > 1) {
		set_current_tile(destination_tile);
		get_transform()->position = current_tile->get_transform()->position;
	}

	if (destination_tile != nullptr)
	{
		get_transform()->position = tile::position_lerp(current_tile, destination_tile, movement_progress);
	}
	
	if (!falling_into_trap)
	{
		tile *hero_tile = he->get_current_tile();
		point dir = find_move_to(hero_tile);

		tile * new_destination = current_tile->get_neighbor(dir);

		if (dir * previous_dir == point())
			movement_progress = 0;
		else if (destination_tile != new_destination && dir * previous_dir != point())
		{
			movement_progress = -movement_progress;
		}

		destination_tile = new_destination;

		
		previous_dir = dir;
	}
	
}
