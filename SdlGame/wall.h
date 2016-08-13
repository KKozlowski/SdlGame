#pragma once
#include "tile.h"

class wall : public tile
{
public:
	wall(int x, int y, level_grid* l);
};
