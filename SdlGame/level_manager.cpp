#include "level_manager.h"
#include "file_reader_line_by_line.h"

std::vector<std::string> level_manager::filenames {"level_one.txt", "level_two.txt" };

bool level_manager::load_level(int id, int tilesize)
{
	if (current_level != nullptr)
		close_level();

	if (id >= filenames.size())
		return false;

	file_reader_line_by_line *li = new file_reader_line_by_line(filenames[id]);
	if (li->is_empty()) 
		return false;

	current_level = new level_grid(li, tilesize, this);
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

bool level_manager::load_next_level()
{
	return load_level(current_level_id + 1, last_tilesize);
}
