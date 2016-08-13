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
	float movement_progress;
public:
	hero(tile *start_tile, level_grid * lg);
	~hero();
	bool can_jump_to_destination() const;
	virtual void update() override;
};
