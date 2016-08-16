#pragma once
#include "rigidbody.h"

class tile;
class level_grid;

class hero : public actor
{
protected:
	tile *current_tile;
	tile *destination_tile;

	point previous_dir;
	level_grid *m_levelgrid;
	float movement_progress;

	bool m_falling;
	bool m_alive = true;
	bool m_winning = false;

	///<summary>
	///Points gathered by player by collecting gold piles.
	///</summary>
	int m_points = 0;

	float movement_speed;

	float adjustment_jump_tolerance;
	///<summary>
	///A two-dimensional difference between a proper movement path 
	///(interpolated between current tile and destination tile), and the
	///actual movement path.
	///<para />It is set when movement direction changes completely
	///(e.g. from horizontal to vertical).
	///</summary>
	vector2f position_offset;
	float offset_reduction_speed = 4.f;

	///<summary>
	///Sets destination tile basing on the given direction (dir : point).
	///When with_jump == true, it performs adjustment jump,
	///setting destination_tile as current tile.
	///</summary>
	void set_direction(point dir, bool with_jump);
	void reduce_offset();

	///<summary>
	///Sets a new tile as the current one. It also checks if the tile has special properties.
	///<para />It calls falling, calls taking gold, calls level end, depending on what the tile is.
	///</summary>
	void set_current_tile(tile *t);

	///<summary>
	///Moves the character between current tile and destination tile. After reaching the destination_tile,
	///it calls set_current_tile.
	///</summary>
	void continue_movement();

	bool dig(point direction);

public:
	hero(tile *start_tile, level_grid * lg);
	~hero();
	///<summary>
	/// If it is close enough to destination tile position, to set it as current tile.
	/// It is important when the hero is going to change direction (e.g. from horizontal
	/// to vertical). <para /> 
	/// To make movement more fluid, he can "skip" some distance and,
	/// for example, start climbing a ladder while standing next to it, not exactly on it.
	///</summary>
	bool can_jump_to_destination() const;
	bool is_alive() const { return m_alive; }
	bool has_won() const { return m_winning; }

	int get_points() const { return m_points; }
	void add_points(int to_add) { m_points += to_add; }

	void die();
	tile *get_current_tile() const;

	virtual void update() override;
};
