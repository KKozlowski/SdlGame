#pragma once
#include "actor.h"

class level_grid;
class tile;

class enemy : public actor
{
private:
	level_grid *m_levelgrid;

	tile *current_tile;
	tile *destination_tile;

	point current_tile_indices;
	point destination_tile_indices;

	float m_speed;
	float movement_progress = 0;

	float killing_range = 0.5f;

	int held_points = 0;

	void set_current_tile(tile *t);
	void set_destination_tile(tile *t);

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
	void reload_key_tiles();
};
