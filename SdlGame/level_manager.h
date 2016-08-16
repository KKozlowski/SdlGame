#pragma once
#include "level_grid.h"

class level_manager : public actor
{
private:
	level_grid *current_level = nullptr;
	int m_currentLevelID = 0;
	int m_tilesize = 160;
	static std::vector<std::string> filenames;

	bool m_atTitleScreen = true;
	float m_timeOfReset = 0;
	float m_timeOfExit = 0;

	bool m_finished = false;

public:
	level_manager(int tilesize);

	level_grid *get_current() const { return current_level; }

	bool load_level(int id);
	bool close_level();
	bool reset_level();
	void reset_level_after_time(float delay);
	bool load_next_level();

	bool is_finished() const { return m_finished; }
	
	void update() override;

	int get_current_id() const { return m_currentLevelID; }
};

