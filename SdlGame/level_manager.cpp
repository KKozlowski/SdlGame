#include "level_manager.h"
#include "file_reader_line_by_line.h"
#include "engine.h"
#include "scene.h"
#include "text_render.h"
#include "title_screen.h"

std::vector<std::string> level_manager::filenames {"level_one.txt", "level_two.txt", "level_three.txt", "level_four.txt" };

bool level_manager::load_level(int id)
{
	m_atTitleScreen = false;
	engine::get_instance()->get_scene()->remove_actor(m_titleScreen);
	if (current_level != nullptr)
		close_level();

	if (id >= filenames.size())
		return false;

	file_reader_line_by_line *li = new file_reader_line_by_line(filenames[id]);
	if (li->is_empty()) 
		return false;

	current_level = new level_grid(li, m_tilesize, this);
	m_currentLevelID = id;

	return true;
}

level_manager::level_manager(int tilesize)
{
	m_tilesize = tilesize;
	//m_draw = new draw_title_screen(this);
	m_titleScreen = new title_screen();
	engine::get_instance()->get_scene()->add_actor(m_titleScreen);

	m_bottomText = new text_render("FFFFFFFFFFFFFFFFFFFFFFF", point(10, engine::get_instance()->get_renderer()->SCREEN_HEIGHT - 50), 50);
	engine::get_instance()->get_scene()->add_actor(m_bottomText);
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
	return load_level(m_currentLevelID + 1);
}

void level_manager::update()
{
	if (m_atTitleScreen)
	{
		m_bottomText->content = "PRESS 'ENTER' TO START";

		if (engine::get_instance()->get_input()->get_key_down(SDLK_RETURN))
			load_level(1);
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
					m_bottomText->content = "GOOD JOB!";
				}
					
			}
		}

		if (m_timeOfReset != 0 && m_timeOfReset < engine::get_time_from_start())
		{
			reset_level();
			m_timeOfReset = 0;
		}

		if (m_timeOfExit != 0 && m_timeOfExit < engine::get_time_from_start())
		{
			m_finished = true;
		}
	}
}
