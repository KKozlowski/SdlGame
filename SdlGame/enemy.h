#pragma once
#include "actor.h"

class level_grid;
class tile;

class enemy : public actor
{
private:
	tile *current_tile;
	tile *destination_tile;
	level_grid *m_levelgrid;

	float m_speed;
	float movement_progress = 0;

	float killing_range = 0.5f;

	int held_points = 0;

	void set_current_tile(tile *t);

	point previous_dir;

	bool falling_into_trap;
	bool falling = false;

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
