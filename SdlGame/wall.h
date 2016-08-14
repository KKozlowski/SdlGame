#pragma once
#include "tile.h"

class wall : public tile
{
private:
	bool digged = false;
	float time_to_refill;
	void refill();
public:
	wall(int x, int y, level_grid* l);
	void dig();
	bool is_empty() override;
	void update() override;
};
