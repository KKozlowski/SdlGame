#include "enemy.h"
#include "draw_texture.h"
#include "tile.h"
#include "level_grid.h"
#include "engine.h"
#include "gold.h"

void enemy::set_current_tile(tile* t)
{
	if (!m_fellIntoTrap)
	{
		if (t->can_down(true) && t->empty_over_empty())
		{
			m_falling = true;
			set_destination_tile(t->get_down());
		}

		if (m_falling && !t->empty_over_empty())
		{
			m_falling = false;
		}
	}
	else
	{
		if (m_gettingOutOfTrap)
		{
			std::cout << "GETTING OUT OF TRAP\n";
			
			m_gettingOutOfTrap = false;
			m_fellIntoTrap = false;
		}
		//std::cout << "IN TRAP\n";
	}

	if (t->can_down() && !t->can_down(true)) //TRAP!
	{
		fall_into_trap(t);
	}

	try_steal_gold(t);

	m_movementProgress -= 1;

	tile_traveller::set_current_tile(t);
}

void enemy::fall_into_trap(tile* tile_youre_on)
{
	std::cout << "FALLING INTO TRAP\n";
	m_fellIntoTrap = true;
	try_drop_gold(tile_youre_on);

	set_destination_tile(tile_youre_on->get_down());
	trap_tile = static_cast<wall *>(m_destinationTile);
	if (trap_tile != nullptr)
	{
		trap_tile->enemy_in_the_hole = this;
	}
		

	m_unstunTime = engine::get_time_from_start() + m_trapStunTime;
}

void enemy::get_out_of_trap()
{
	if (!m_gettingOutOfTrap)
	{
		m_movementProgress = 0;

		if (trap_tile != nullptr)
			trap_tile->enemy_in_the_hole = nullptr;

		//Look for a good destination
		point distance = get_2d_distance_to_tile(m_levelgrid->get_hero()->get_current_tile());
		tile *up = m_currentTile->get_up();
		if (distance.x > 0 && up->can_right())
			set_destination_tile(up->get_right());
		else if (distance.x < 0 && up->can_left())
			set_destination_tile(m_currentTile->get_up()->get_left());
		else return;

		m_gettingOutOfTrap = true;
	}
	
}

bool enemy::try_steal_gold(tile* t)
{
	if (!m_fellIntoTrap && t->get_gold() != nullptr)
	{
		m_heldPoints += t->get_gold()->get_value();
		t->pop_gold();
		return true;
	}

	return false;
}

bool enemy::try_drop_gold(tile* t)
{
	if (m_heldPoints > 0)
	{
		m_levelgrid->put_gold_on_tile(t, m_heldPoints);
		m_heldPoints = 0;
		return true;
	}
	return false;
}

tile* enemy::find_vertical_passage(point dir)
{
	tile *next_tile = m_currentTile;

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
			return next_tile;
		}

		if (dir.y>0 && next_tile->can_down(true))
		{
			return next_tile;
		}
			
	} while (true);

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
		if (abs(m_currentTile->get_Xpos() - passage_one->get_Xpos()) 
			< abs(m_currentTile->get_Xpos() - passage_two->get_Xpos()))
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
		t->get_Xpos() - m_currentTile->get_Xpos(),
		t->get_Ypos() - m_currentTile->get_Ypos()
		);
}

point enemy::find_move_to(tile* t)
{
	point result(0, 0);
	point where_to_go = get_2d_distance_to_tile(t);
	if (m_falling)
		return{ 0,1 };
	if (where_to_go.y == 0) //WE ARE ON THE SAME LEVEL!
	{
		tile *next_tile = m_currentTile;
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
			if (m_currentTile->can_up()) result = { 0,-1 };

			else
			{
				passage = find_closest_vertical_passage({ 0,-1 });
			}
		}
		else
		{
			if (m_currentTile->can_down()) result = { 0,1 };
			else
			{
				passage = find_closest_vertical_passage({ 0,1 });
			}
		}

		if (passage != nullptr)
		{
			if (passage->get_Xpos() > m_currentTile->get_Xpos())
				result = { 1,0 };
			if (passage->get_Xpos() < m_currentTile->get_Xpos()) //; in memory of the most evil semicolon ever.
				result = { -1,0 };
		}
	}

	return result;
}

void enemy::die()
{
	std::cout << "ENEMY DIES\n";
	try_drop_gold(m_currentTile);
	m_levelgrid->on_enemy_death(this);
}

enemy::enemy(tile* start_tile, level_grid* lg)
	:tile_traveller(lg, 1.5f)
{
	m_levelgrid = lg;
	draw_texture *dt = new draw_texture(this, "enemy.png");
	dt->set_width_height(lg->get_tilesize(), lg->get_tilesize());
	dt->centered = true;
	m_draw = dt;

	enemy::set_current_tile(start_tile);
	get_transform()->position = start_tile->get_transform()->position;
}

enemy::~enemy()
{
	if (trap_tile != nullptr && trap_tile->enemy_in_the_hole == this)
		trap_tile->enemy_in_the_hole = nullptr;

}

void enemy::update(){

	if (!m_gettingOutOfTrap && m_currentTile->is_death_trap())
	{
		die();
		return;
	}

	hero *he = m_levelgrid->get_hero();
	if ((get_transform()->position - he->get_transform()->position).length() < m_killingRange*m_levelgrid->get_tilesize())
	{
		he->die();
	}

	m_movementProgress += m_movementSpeed * engine::get_delta_time();
	if (m_movementProgress >= 1 && m_destinationTile != nullptr) {
		set_current_tile(m_destinationTile);
		if (!m_fellIntoTrap && !m_gettingOutOfTrap)
			set_destination_tile(m_currentTile->get_neighbor(m_previousDirection));
		get_transform()->position = m_currentTile->get_transform()->position;
	}

	if (m_destinationTile != nullptr)
	{
		get_transform()->position = tile::position_lerp(m_currentTile, m_destinationTile, m_movementProgress);
	}

	if (m_fellIntoTrap)
	{
		if (engine::get_time_from_start() > m_unstunTime)
			get_out_of_trap();
	}
	else if (!m_gettingOutOfTrap)
	{
		tile *hero_tile = he->get_current_tile();
		point dir = find_move_to(hero_tile);

		tile * new_destination = m_currentTile->get_neighbor(dir);

		if (dir * m_previousDirection == point())
			m_movementProgress = 0;
		else if (m_destinationTile != new_destination && dir * m_previousDirection != point())
		{
			m_movementProgress = -m_movementProgress;
		}

		set_destination_tile(new_destination);

		m_previousDirection = dir;
	}

}
