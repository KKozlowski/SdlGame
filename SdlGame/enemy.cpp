#include "enemy.h"
#include "draw_texture.h"
#include "tile.h"
#include "level_grid.h"
#include "engine.h"
#include "gold.h"

void enemy::set_current_tile(tile* t)
{
	if (m_fellIntoTrap)
	{
		if (m_gettingOutOfTrap)
		{
			std::cout << "ENEMY GETS OUT OF TRAP\n";

			m_gettingOutOfTrap = false;
			m_fellIntoTrap = false;
		}
	}
	else
	{
		if (t->can_down(true) && t->empty_over_empty() && t->get_down()->get_type() != tile_type::wall)
		{
			m_naturalFalling = true;
			set_destination_tile(t->get_down());
		}

		if (m_naturalFalling && (!t->empty_over_empty() || t->get_down()->get_type() == tile_type::wall))
		{
			m_naturalFalling = false;
		}
	}

	if (t->can_down() && !t->can_down(true)) //TRAP!
	{
		fall_into_trap(t);
	}

	try_steal_gold(t);

	m_movementProgress -= 1;

	tile_traveller::set_current_tile(t);
}

void enemy::set_destination_tile(tile* t)
{
	if (m_destinationTile != nullptr && m_destinationTile->enemy_destined_here == this) // "Free" the previous destination tile
		m_destinationTile->enemy_destined_here = nullptr;

	if (is_during_regular_movement() 
		&& t->enemy_destined_here != nullptr) // It the new destination tile is taken, and you aren't doing anything special, stop for a moment.
		return;
	

	tile_traveller::set_destination_tile(t);

	if (is_during_regular_movement())
		m_destinationTile->enemy_destined_here = this;
}

void enemy::fall_into_trap(tile* tile_youre_on)
{
	wall *trap = static_cast<wall *>(tile_youre_on->get_down());

	if (trap->enemy_in_the_hole != nullptr)
		return;

	std::cout << "ENEMY FALLS INTO TRAP\n";
	m_fellIntoTrap = true;
	try_drop_gold(tile_youre_on);

	set_destination_tile(trap);
	trap_tile = trap;

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
		if (distance.x > 0 && up->can_right(true))
			set_destination_tile(up->get_right());
		else if (distance.x < 0 && up->can_left(true))
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
	if (m_heldPoints > 0 && m_levelgrid != nullptr)
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
	return t->get_indices() - m_currentTile->get_indices();
}

point enemy::find_move_to(tile* t)
{
	point result(0, 0);
	point where_to_go = get_2d_distance_to_tile(t);
	if (m_naturalFalling)
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
		point direction = { 0, 
			                where_to_go.y > 0 ? 1 : -1 
		                  };

		if (m_currentTile->can(direction, true)) 
			result = direction;
		else
		{
			passage = find_closest_vertical_passage(direction);
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
	if (m_destinationTile->enemy_destined_here == this)
		m_destinationTile->enemy_destined_here = nullptr;
	std::cout << "ENEMY DIES\n";
	try_drop_gold(m_currentTile);

	if (m_levelgrid != nullptr)
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
	//Clearing the trap you are in
	if (trap_tile != nullptr && trap_tile->enemy_in_the_hole == this)
		trap_tile->enemy_in_the_hole = nullptr;
}

void enemy::update(){
	if (m_levelgrid == nullptr) 
		return;

	hero *he = m_levelgrid->get_hero();
	
	//KILLING HERO
	if ((get_transform()->position - he->get_transform()->position).length() < m_killingRange*m_levelgrid->get_tilesize())
	{
		he->die();
	}

	//CONTINUING MOVEMENT
	m_movementProgress += m_movementSpeed * engine::get_delta_time();

	if (m_movementProgress >= 1 && m_destinationTile != nullptr) 
	{
		set_current_tile(m_destinationTile);

		if (!m_fellIntoTrap && !m_gettingOutOfTrap)
			set_destination_tile(m_currentTile->get_neighbor(m_previousDirection));

		get_transform()->position = m_currentTile->get_transform()->position;
	}

	// *LERPING BETWEEN CURRENT AND DESTINATION TILES
	if (m_destinationTile != nullptr)
	{
		get_transform()->position = tile::position_lerp(m_currentTile, m_destinationTile, m_movementProgress);
	}

	if (m_fellIntoTrap)
	{
		//Checking if I can go out of trap yet
		if (engine::get_time_from_start() > m_unstunTime)
			get_out_of_trap();
	}
	else if (!m_gettingOutOfTrap)
	{
		//Looking for a right move to get to the hero position
		tile *hero_tile = he->get_current_tile();
		point dir = find_move_to(hero_tile);

		tile * new_destination = m_currentTile->get_neighbor(dir);


		point zero(0, 0);

		//Handling direction change
		if (dir * m_previousDirection == zero) //Complete change
			m_movementProgress = 0;
		else if (m_destinationTile != new_destination && dir * m_previousDirection != zero) //Direction changed, but not the axis
		{
			m_movementProgress = -m_movementProgress;
		}

		set_destination_tile(new_destination);

		m_previousDirection = dir;
	}

}
