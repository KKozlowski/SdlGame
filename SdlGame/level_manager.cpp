#include "level_manager.h"
#include "file_reader_line_by_line.h"
#include "engine.h"
#include "scene.h"
#include "text_render.h"
#include "title_screen.h"
#include <sstream>
#include "ui.h"

std::vector<std::string> level_manager::filenames {"level_one.txt", "level_two.txt", "level_three.txt", "level_four.txt" };

bool level_manager::load_level(int id)
{
	m_atTitleScreen = false;
	m_timeOfLevelLoad = 0;
	engine::get_instance()->get_ui()->hide_title_screen();

	if (current_level != nullptr)
		close_level();

	if (id >= filenames.size())
		return false;

	m_timeOfReset = 0;

	file_reader_line_by_line *li = new file_reader_line_by_line(filenames[id]);
	if (li->is_empty()) 
		return false;

	current_level = new level_grid(li, m_tilesize, this);
	m_currentLevelID = id;

	return true;
}

bool level_manager::load_level(int id, float delay)
{
	if (id >= filenames.size())
		return load_level(id);

	engine::get_instance()->get_ui()->hide_title_screen();
	m_atTitleScreen = false;

	close_level();
	m_nextLevelLoadId = id;
	m_timeOfLevelLoad = engine::get_time_from_start() + delay;

	std::stringstream ss;
	ss << "LEVEL " << id;

	engine::get_instance()->get_ui()->set_score_text(ss.str());

	return true;
}

level_manager::level_manager(int tilesize)
{
	m_tilesize = tilesize;
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
	m_timeOfReset = 0;

	if (current_level != nullptr)
	{
		close_level();
		load_level(m_currentLevelID);

		return true;
	}
	return false;
}

void level_manager::reset_level_after_time(float delay)
{
	m_timeOfReset = engine::get_time_from_start() + delay;
}

bool level_manager::load_next_level()
{
	std::cout << "LOADING NEXT LEVEL\n";

	return load_level(m_currentLevelID + 1, 1.f);
}

void level_manager::update()
{
	if (m_atTitleScreen)
	{
		engine::get_instance()->get_ui()->set_score_text("PRESS 'ENTER' TO START");

		if (engine::get_instance()->get_input()->get_key_down(SDLK_RETURN))
			load_level(1, 1.f);
	} else
	{
		hero *he = nullptr;
		if (get_current())
		{
			he = get_current()->get_hero();
			get_current()->update();
		}
		if (he != nullptr)
		{
			if (!he->is_alive() && m_timeOfReset == 0)
			{
				reset_level_after_time(2.f);
			}
			else if (he->has_won() || engine::get_instance()->get_input()->get_key_down(SDLK_DOWN))
			{
				if (!load_next_level())
				{
					m_timeOfExit = engine::get_time_from_start() + 3.f;
					engine::get_instance()->get_ui()->set_score_text("GOOD JOB!");
				}
					
			}
		}

		if (m_timeOfReset != 0 && m_timeOfReset < engine::get_time_from_start())
		{
			reset_level();
		}

		if (m_timeOfExit != 0 && m_timeOfExit < engine::get_time_from_start())
		{
			m_finished = true;
		}

		if (m_timeOfLevelLoad != 0 && m_timeOfLevelLoad < engine::get_time_from_start())
		{
			load_level(m_nextLevelLoadId);
		}
	}
}
