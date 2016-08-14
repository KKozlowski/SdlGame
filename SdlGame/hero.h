#pragma once
#include "ball.h"
#include "rigidbody.h"

class tile;
class level_grid;

class hero : public ball
{
protected:
	float move_speed = 210;
	tile *current_tile;
	tile *destination_tile;
	point previous_dir;
	level_grid *m_levelgrid;
	float movement_progress;

	bool falling;
	bool alive = true;

	int points = 0;

	float speed = 2.f;
	float adjustment_jump_tolerance = 0.25f;

	void go_by_direction(point dir, bool with_jump);
	vector2f temporal_offset;
	float offset_reduction_speed = 4.f;
	void reduce_offset();

	void set_current_tile(tile *t);

	void continue_movement();

	bool dig(point direction);
public:
	hero(tile *start_tile, level_grid * lg);
	~hero();
	bool can_jump_to_destination() const;
	bool is_alive() const { return alive; }

	int get_points() const { return points; }
	void add_points(int to_add) { points += to_add; }

	void die();
	tile *get_current_tile() const;

	virtual void update() override;
};
