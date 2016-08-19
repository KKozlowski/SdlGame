#pragma once
#include "actor.h"
#include "tile_traveller.h"
#include "wall.h"

class level_grid;
class tile;

class enemy : public tile_traveller
{
private:
	float m_trapStunTime = 2.4f;

	float m_killingRange = 0.8f;

	int m_heldPoints = 0;

	bool m_fellIntoTrap = false;
	bool m_gettingOutOfTrap = false;
	bool m_falling = false;
	float m_unstunTime = 0;
	wall *trap_tile = nullptr;

	void set_current_tile(tile *t) override;
	void set_destination_tile(tile *t) override;
	void fall_into_trap(tile *tile_youre_on);
	void get_out_of_trap();

	bool try_steal_gold(tile *t);
	bool try_drop_gold(tile *t);

	tile *find_vertical_passage(point dir);
	tile *find_closest_vertical_passage(point dir);

	point get_2d_distance_to_tile(tile *t);
	point find_move_to(tile *t);
public:
	void die();
	enemy(tile *start_tile, level_grid *lg);
	~enemy();
	void update() override;

	bool is_during_regular_movement() const { return !m_fellIntoTrap && !m_falling&& !m_gettingOutOfTrap; }
};
