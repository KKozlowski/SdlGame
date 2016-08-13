﻿#pragma once
#include "ball.h"
#include "rigidbody.h"

class tile;
class level_grid;

class hero : public ball
{
protected:
	float move_speed = 210;
	tile *current_tile;
	tile *destination_tile;
	vector2f previous_dir;
	level_grid *m_levelgrid;
	float movement_progress;
	bool falling;

	float speed = 2.f;
	float adjustment_jump_tolerance = 0.25f;

	void set_current_tile(tile *t);
public:
	hero(tile *start_tile, level_grid * lg);
	~hero();
	bool can_jump_to_destination() const;
	virtual void update() override;
};
