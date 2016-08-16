#pragma once
#include "tile.h"

class enemy;

class wall : public tile
{
private:
	bool digged = false;
	bool destructible;
	float time_to_refill;
	void refill();
public:
	enemy *enemy_in_the_hole = nullptr;

	wall(int x, int y, level_grid* l, bool destructible = true);
	void dig();
	bool is_empty() override;
	bool is_destructible() const { return destructible; }
	void update() override;
};
