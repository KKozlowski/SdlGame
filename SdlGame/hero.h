#pragma once
#include "rigidbody.h"
#include <iostream>
#include "tile_traveller.h"

class tile;
class level_grid;

class hero : public tile_traveller
{
protected:
	float m_diggingTime = 0.25f;

	bool m_falling;
	bool m_alive = true;
	bool m_winning = false;
	float m_timeOfDiggingStop = 0;

	///<summary>
	///Points gathered by player by collecting gold piles.
	///</summary>
	int m_goldenPoints = 0;

	///<summary>
	///Points gathered by player by killing the enemies.
	///</summary>
	int m_killerPoints = 0;

	float adjustment_jump_tolerance;
	///<summary>
	///A two-dimensional difference between a calculated movement path 
	///(interpolated between current tile and destination tile), and the
	///actual movement path.
	///<para />It is set when movement direction changes completely
	///(e.g. from horizontal to vertical).
	///</summary>
	vector2f position_offset;
	float offset_reduction_speed = 4.f;

	void reduce_offset();

	///<summary>
	///Sets destination tile basing on the given direction (dir : point).
	///When with_jump == true, it sets destination_tile as current tile.
	///</summary>
	void set_direction(point dir, bool with_jump);
	///<summary>
	///Sets a new tile as the current one. It also checks if the tile has special properties.
	///<para />It calls falling, calls taking gold, calls level end, depending on what the tile is.
	///</summary>
	void set_current_tile(tile *t) override;

	///<summary>
	///Moves the character between current tile and destination tile. After reaching the destination_tile,
	///it calls set_current_tile.
	///</summary>
	void continue_movement();

	bool dig(point direction);

	///<summary>Calls character to move horizontally. side = 1 - right; side = -1 = left</summary>
	point move_side(int side);
	point move_down();
	point move_up();

	void handle_direction_change(point dir);

public:
	hero(tile *start_tile, level_grid * lg);
	~hero();
	///<summary>
	/// Returns if hero is close enough to destination tile position, to set it as current tile.
	/// It is important when the hero is going to change direction (e.g. from horizontal
	/// to vertical). <para /> 
	/// To make movement more fluid, hero can "skip" some distance and,
	/// for example, start climbing a ladder while standing next to it, not exactly on it.
	///</summary>
	bool can_jump_to_destination() const;
	bool is_alive() const { return m_alive; }
	bool is_digging() const { return m_timeOfDiggingStop != 0; }
	bool has_won() const { return m_winning; }

	void stop_digging();

	int get_golden_points() const { return m_goldenPoints; }
	void add_golden_points(int to_add) { m_goldenPoints += to_add; }

	int get_killer_points() const { return m_killerPoints; }
	void add_killer_points(int to_add) { m_killerPoints += to_add; }

	int get_total_points() const { return m_goldenPoints + m_killerPoints; }

	void die();

	virtual void update() override;

	///<summary>
	/// Reads player input, calculates wanted direction from input.
	/// Calls set_direction for calculated direction.
    /// Also starts falling process when it's possible and wanted by player.
	///</summary>
	point read_and_apply_input();
};
