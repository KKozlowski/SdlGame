#pragma once
#include "tile.h"

class enemy;

class wall : public tile
{
private:
	bool digged = false;
	bool destructible;
	float time_to_refill;
	float m_timeOfDig = 0;
	static float rebuilding_time;
	void refill();
public:
	enemy *enemy_in_the_hole = nullptr;

	wall(int x, int y, level_grid* l, bool destructible = true);
	bool dig(float delay = 0);
	bool is_empty() override;
	bool is_destructible() const { return destructible; }
	void update() override;
};
