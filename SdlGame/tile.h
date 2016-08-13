#pragma once
#include "actor.h"
#include "vector2.h"
#include "level_grid.h"
#include "draw_texture.h"

class tile : public actor
{
private:
	level_grid *m_level;
protected:
	point indices;
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

	virtual bool is_accessible()
	{
		return false;
	}

	draw_texture *get_tex_draw() const
	{
		return static_cast<draw_texture*>(drawing);
	}
};
