﻿#pragma once
#include <string>
#include "vector2.h"
#include "tileset.h"
#include "hero.h"
#include <vector>

class level_manager;

class level_grid
{
private:
	hero *m_hero;
	float m_tilesize;
	level_manager *m_levelmanager;

	std::vector<point> hidden_ladders;
	std::vector<gold *> gold_piles;
	point final_ladder;

	bool during_deconstruction = false;

	std::vector<std::vector<tile*> *> *tile_grid = new std::vector<std::vector<tile*>*>();

	void unhide_ladder(point indices, bool finalizing);
public:
	level_grid(std::string filename, float tilesize, level_manager *manager);
	~level_grid();
	hero *get_hero() const { return m_hero; }
	float get_tilesize() const { return m_tilesize; }
	level_manager *get_levelmanager() const { return m_levelmanager; }

	tile *get(int column, int row);
	void put_gold_on_tile(tile * tile, int value);

	void at_gold_disappearance(gold *g);
};
