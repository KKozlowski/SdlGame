﻿#pragma once
#include <string>
#include "vector2.h"
#include "tileset.h"
#include "hero.h"
#include <vector>
#include "file_reader_line_by_line.h"
#include <queue>

class level_manager;
class enemy;

///<summary>
/// A class that reads level data (symbol after symbol). It spawns tiles,
/// enemies, hero, and gold. It also handles enemy respawning.<para />
/// At destruction, it removes all its child actors, inluding hero.
/// Destruction should be called after hero death.
///</summary>
class level_grid
{
private:
	level_manager *m_levelmanager;
	float m_tilesize;
	float m_enemyRespawnTime = 4.f;
	float m_minimumTimeBetweenEnemyRespawns = 1.f;

	hero *m_hero;

	std::vector<point> hidden_ladders;
	std::vector<gold *> gold_piles;
	std::vector<enemy *> enemies;
	point final_ladder;
	point spawner;

	int required_gold;

	std::deque<float> m_respawnMoments;

	bool during_deconstruction = false;

	std::vector<std::vector<tile*> *> *tile_grid = new std::vector<std::vector<tile*>*>();

	void unhide_ladder(point indices, bool finalizing);

	void set_score_text(int pts);

	tile *get_respawner_tile();
	void call_respawn();
	void handle_respawns();
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
	bool on_hero_gold_take(int golden_points, int killer_points);
	void on_gold_disappearance(gold *g);
	void on_enemy_death(enemy *e);

	//Remember that level_grid is not an actor.
	void update();
};
