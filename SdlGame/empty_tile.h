#pragma once
#include "tile.h"

class empty_tile : public tile
{
public:
	empty_tile(int x, int y, level_grid* l);
};
