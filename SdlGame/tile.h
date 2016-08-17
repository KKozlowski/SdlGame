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
	point m_indices;

protected:
	tile_type m_type;

public:
	///<summary> Returns horizontal index of the tile in the grid. </summary>
	int get_Xpos() const { return m_indices.x; }
	///<summary> Returns vertical index of the tile in the grid. </summary>
	int get_Ypos() const { return m_indices.y; }
	///<summary> Returns horizontal (x) and vertical (y) indices of the tile in the grid. </summary>
	point get_indices() const { return m_indices; }

	level_grid *get_levelgrid() const { return m_level; }

	tile(int x, int y, level_grid* l);

	virtual ~tile() {  }

	virtual bool is_accessible()
	{
		return false;
	}

	draw_texture *get_tex_draw() const //All the tiles have draw_texture as their basic draw.
	{
		return static_cast<draw_texture*>(m_draw);
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

	virtual bool is_empty() { return false; }

	virtual bool can_up(bool for_ai = false);
	virtual bool can_down(bool for_ai = false);
	
	virtual bool can_left(bool for_ai = false);
	virtual bool can_right(bool for_ai = false);

	virtual bool ends_the_level() const { return false; }

	bool over_empty();
	bool empty_over_empty();
	bool over_solid();

	bool set_gold(gold *);
	gold *get_gold() const;
	void pop_gold();

	static vector2f position_lerp(tile *start, tile *end, float percent);
};
