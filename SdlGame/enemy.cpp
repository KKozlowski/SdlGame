﻿#include "enemy.h"
#include "draw_texture.h"
#include "tile.h"
#include "level_grid.h"
#include "engine.h"
#include "gold.h"

void enemy::set_current_tile(tile* t)
{
	if (t->can_down() && !t->can_down(true)) //TRAP!
	{
		falling_into_trap = true;
		try_drop_gold(t);

		destination_tile = t->get_down();
		wall *our_doom = static_cast<wall *>(destination_tile);
		if (our_doom != nullptr)
			our_doom->enemy_in_the_hole = this;
	}
	if (!falling_into_trap)
	{
		if (t->can_down(true) && t->empty_over_empty())
		{
			falling = true;
			destination_tile = t->get_down();
		}

		if (falling && !t->empty_over_empty())
		{
			falling = false;
		}
	}
	

	try_steal_gold(t);

	current_tile = t;

	//std::cout << "TILE NOWWW: " << current_tile->get_indices().to_string() << std::endl;

	movement_progress -= 1;
}

bool enemy::try_steal_gold(tile* t)
{
	if (!falling_into_trap && t->get_gold() != nullptr)
	{
		held_points += t->get_gold()->get_value();
		t->pop_gold();
		return true;
	}

	return false;
}

bool enemy::try_drop_gold(tile* t)
{

	if (held_points > 0)
	{
		//wwwstd::cout << held_points << std::endl;
		m_levelgrid->put_gold_on_tile(t, held_points);
		return true;
	}
	return false;
}

tile* enemy::find_vertical_passage(point dir)
{
	tile *next_tile = current_tile;

	do {
		if (dir.x > 0 && next_tile->can_right(true)) {
			next_tile = next_tile->get_right();
		}
		else if (dir.x < 0 && next_tile->can_left(true))
		{
			next_tile = next_tile->get_left();
		}
		else break;

		if (dir.y<0 && next_tile->can_up(true))
		{
			//std::cout << next_tile->get_indices().to_string() << std::endl;
			return next_tile;
		}
			

		if (dir.y>0 && next_tile->can_down(true))
		{
			//std::cout << next_tile->get_indices().to_string() << std::endl;
			return next_tile;
		}
			
	} while (true);

	//std::cout << "null" << std::endl;
	return nullptr;
}

tile* enemy::find_closest_vertical_passage(point dir)
{
	tile *passage_one = find_vertical_passage({ -1, dir.y });
	tile *passage_two = find_vertical_passage({ 1, dir.y });

	if (passage_one == nullptr)
		return passage_two;
	
	if (passage_two == nullptr)
		return passage_one;

	if (passage_one != nullptr && passage_two != nullptr)
	{
		if (abs(current_tile->get_Xpos() - passage_one->get_Xpos()) 
			< abs(current_tile->get_Xpos() - passage_two->get_Xpos()))
		{
			return passage_one;
		}
		return passage_two;
	}

	return nullptr;
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
	if (falling)
		return{ 0,1 };
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
		}
		else if (where_to_go.x > 0)
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
	else //WE HAVE TO MOVE VERTICALLY!
	{
		tile *passage = nullptr;
		if (where_to_go.y < 0)
		{
			if (current_tile->can_up()) result = { 0,-1 };

			else
			{
				passage = find_closest_vertical_passage({ 0,-1 });
			}
		}
		else
		{
			if (current_tile->can_down()) result = { 0,1 };
			else
			{
				passage = find_closest_vertical_passage({ 0,1 });
			}
		}

		if (passage != nullptr)
		{
			if (passage->get_Xpos() > current_tile->get_Xpos())
				result = { 1,0 };
			if (passage->get_Xpos() < current_tile->get_Xpos()) //; in memory of the most evil semicolon ever.
				result = { -1,0 };

				//std::cout << "\n\nPASSAGE LOCATION: " << passage->get_indices().to_string() << "\nCURRENT TILE: " << current_tile->get_indices().to_string() << "\nRESULT: " << result.to_string() << std::endl;
		}


		//std::cout << result.to_string() << std::endl;
	}

	return result;
}

void enemy::die()
{
	m_draw->set_visible(false);
}

enemy::enemy(tile* start_tile, level_grid* lg)
{
	m_levelgrid = lg;
	draw_texture *dt = new draw_texture(this, "enemy.png");
	dt->set_width_height(lg->get_tilesize(), lg->get_tilesize());
	dt->centered = true;
	m_draw = dt;

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
	if (movement_progress > 1 && destination_tile != nullptr) {
		set_current_tile(destination_tile);
		if (!falling_into_trap)
			destination_tile = current_tile->get_neighbor(previous_dir);
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
