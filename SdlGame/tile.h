#pragma once
#include "actor.h"
#include "vector2.h"
#include "draw_texture.h"
#include "tile_type.h"
#include <iostream>

class level_grid;

class tile : public actor
{
private:
	level_grid *m_level;
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

	virtual bool can_up();
	virtual bool can_down();
	
	virtual bool can_left();
	virtual bool can_right();
};
