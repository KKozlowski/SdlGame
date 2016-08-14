#pragma once
#include "tile.h"

class level_grid;

class ladder : public tile
{
public:
	ladder(int x, int y, level_grid* l);
	bool can_up(bool for_ai = false) override;
};
