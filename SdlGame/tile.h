#pragma once
#include "actor.h"
#include "vector2.h"
#include "draw_texture.h"
#include "tile_type.h"
#include <iostream>

class gold;
class level_grid;

class tile : public actor
{
private:
	level_grid *m_level;
	gold *m_gold;
protected:
	point indices;
	tile_type m_type;
public:
	int get_Xpos() const
	{ return indices.x; }
	int get_Ypos() const
	{ return indices.y; }

	level_grid *get_level() const
	{
		return m_level;
	}

	tile(int x, int y, level_grid* l);

	virtual ~tile() { std::cout << "HUE"; }

	virtual bool is_accessible()
	{
		return false;
	}

	draw_texture *get_tex_draw() const
	{
		return static_cast<draw_texture*>(drawing);
	}

	tile_type get_type() const
	{
		return m_type;
	}

	tile *get_up();
	tile *get_down();
	tile *get_left();
	tile *get_right();
	tile *get_neighbor(point offset);

	virtual bool can_up();
	virtual bool can_down();
	
	virtual bool can_left();
	virtual bool can_right();

	bool over_empty();
	bool empty_over_empty();
	bool over_solid();

	bool set_gold(gold *);
	gold *get_gold() const;
	void pop_gold();

	static vector2f position_lerp(tile *start, tile *end, float percent);
};
