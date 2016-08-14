#pragma once
#include "tile.h"

class level_grid;

class gold : public actor
{
private:
	tile *parent = nullptr;
	int point_value;

public:
	gold(int points);
	//gold(tile *tile, int points);
	bool set_tile(tile *new_parent, level_grid *lg);
	int get_value();
	void set_value(int pts);

};
