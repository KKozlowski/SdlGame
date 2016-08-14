#pragma once
#include <string>
#include "vector2.h"
#include "tileset.h"
#include "hero.h"
#include <vector>

class level_grid
{
private:
	hero *m_hero;
	float m_tilesize;
	std::vector<point> hidden_ladders;
	point final_ladder;

	std::vector<std::vector<tile*> *> *tile_grid = new std::vector<std::vector<tile*>*>();

	void unhide_ladder(point indices, bool finalizing);
public:
	level_grid(std::string filename, float tilesize, point start);
	~level_grid();
	hero *get_hero() const { return m_hero; }
	float get_tilesize() const { return m_tilesize; }

	tile *get(int column, int row);

	void at_gold_disappearance();
};
