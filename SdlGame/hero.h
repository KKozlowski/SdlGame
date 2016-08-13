#pragma once
#include "ball.h"
#include "rigidbody.h"

class tile;
class level_grid;

class hero : public ball
{
protected:
	float move_speed = 210;
	tile *current_tile;
public:
	hero(tile *start_tile, level_grid * lg);
	~hero();
	virtual void update() override;
};
