#include "level_manager.h"

bool level_manager::load_level(int id, int tilesize)
{
	if (current_level != nullptr)
		close_level();

	current_level = new level_grid("level_one.txt", tilesize, this);
	current_level_id = id;

	last_tilesize = tilesize;

	return true;
}

bool level_manager::close_level()
{
	if (current_level != nullptr)
	{
		delete current_level;
		current_level = nullptr;
		return true;
	}
	
	return false;
}

bool level_manager::reset_level()
{
	if (current_level != nullptr)
	{
		close_level();
		load_level(current_level_id, last_tilesize);
		return true;
	}
	return false;
}

void level_manager::update()
{

}
