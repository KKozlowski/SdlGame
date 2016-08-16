#pragma once
#include <string>
#include "vector2.h"
#include "tileset.h"
#include "hero.h"
#include <vector>
#include "file_reader_line_by_line.h"

class level_manager;
class enemy;

class level_grid
{
private:
	hero *m_hero;
	float m_tilesize;
	level_manager *m_levelmanager;

	std::vector<point> hidden_ladders;
	std::vector<gold *> gold_piles;
	std::vector<enemy *> enemies;
	point final_ladder;

	int required_gold;

	bool during_deconstruction = false;

	std::vector<std::vector<tile*> *> *tile_grid = new std::vector<std::vector<tile*>*>();

	void unhide_ladder(point indices, bool finalizing);
public:
	level_grid(file_reader_line_by_line *li, float tilesize, level_manager *manager);
	~level_grid();
	hero *get_hero() const { return m_hero; }
	float get_tilesize() const { return m_tilesize; }
	level_manager *get_levelmanager() const { return m_levelmanager; }

	tile *get(int column, int row);
	void put_gold_on_tile(tile * tile, int value);
	void put_enemy_on_tile(tile *tile);

	int get_required_gold();
	bool on_hero_gold_take(int pts);
	void at_gold_disappearance(gold *g);
};
