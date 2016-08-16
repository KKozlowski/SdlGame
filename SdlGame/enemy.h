#pragma once
#include "actor.h"
#include "tile_traveller.h"

class level_grid;
class tile;

class enemy : public tile_traveller
{
private:

	float killing_range = 0.5f;

	int held_points = 0;

	bool falling_into_trap;
	bool falling = false;

	virtual void set_current_tile(tile *t) override;

	bool try_steal_gold(tile *t);
	bool try_drop_gold(tile *t);

	tile *find_vertical_passage(point dir);
	tile *find_closest_vertical_passage(point dir);

	point get_2d_distance_to_tile(tile *t);
	point find_move_to(tile *t);

	void die();
public:
	enemy(tile *start_tile, level_grid *lg);
	void update() override;
};
