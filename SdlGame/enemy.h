﻿#pragma once
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

	void set_current_tile(tile *t);

	point get_2d_distance_to_tile(tile *t);
public:
	enemy(tile *start_tile, level_grid *lg);
	void update() override;
};
