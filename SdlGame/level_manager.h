#pragma once
#include "level_grid.h"
#include "title_screen.h"

///<summary>
/// Object of this class initializes level grids. 
/// It holds the object of current level grid. <para />
/// It handles level loading, level reset, level unload. <para/>
/// It handles game ending after the last level. <para />
///</summary>
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

	///<summary>
	/// It returns the currently active game level.
	///</summary>
	level_grid *get_current() const { return current_level; }

	///<summary> Loads new level with given id. If level with this id doesn't exist, it returns false. </summary>
	bool load_level(int id);
	///<summary> Unloads current level. If no level is loaded, it returns false. </summary>
	bool close_level();
	///<summary> Resets current level. If no level is loaded, it returns false. </summary>
	bool reset_level();
	///<summary> Resets level after given time in seconds. </summary>
	void reset_level_after_time(float delay);
	///<summary> Loads next level (with id+1). If there are no more levels, it returns false. </summary>
	bool load_next_level();

	///<summary> It is set to true when the object decides it's time to close the app - a few seconds after the last level end. </summary>
	bool is_finished() const { return m_finished; }
	
	void update() override;

	///<summary> Returns id of the current level. </summary>
	int get_current_id() const { return m_currentLevelID; }
};

