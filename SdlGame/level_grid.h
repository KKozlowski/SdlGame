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

	std::vector<std::vector<tile*> *> *tile_grid = new std::vector<std::vector<tile*>*>();
public:
	level_grid(std::string filename, float tilesize, point start);
	hero *get_hero() const { return m_hero; }
	float get_tilesize() const { return m_tilesize; }

	tile *get(int column, int row);
};
