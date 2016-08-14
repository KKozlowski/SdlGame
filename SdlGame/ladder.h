#pragma once
#include "tile.h"

class level_grid;

class ladder : public tile
{
	bool the_final_one = false;
public:
	ladder(int x, int y, level_grid* l, bool ends_the_level = false);
	bool can_up(bool for_ai = false) override;
	bool ends_the_level() const override;
};
