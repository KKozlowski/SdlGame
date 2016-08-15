#pragma once
#include "level_grid.h"

class level_manager
{
private:
	level_grid *current_level = nullptr;
	int current_level_id;
	int last_tilesize;

	
public:
	level_grid *get_current() const { return current_level; }
	
	bool load_level(int id, int tilesize);
	bool close_level();
	bool reset_level();

	void update();
};

