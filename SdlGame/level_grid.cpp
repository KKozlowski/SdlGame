#include "level_grid.h"
#include <fstream>
#include <iostream>
#include "engine.h"
#include "scene.h"
#include "draw_texture.h"
#include "pipe.h"
#include "gold.h"
#include "level_manager.h"
#include "enemy.h"

void level_grid::unhide_ladder(point indices, bool finalizing)
{
	if (get(indices.x, indices.y) == nullptr)
		return;

	engine::get_instance()->get_scene()->remove_actor(
		get(indices.x, indices.y)
		);
	delete get(indices.x, indices.y);

	ladder *l = new ladder(indices.x, indices.y, this, finalizing);

	tile_grid->at(indices.y)->at(indices.x) = l;
	engine::get_instance()->get_scene()->add_actor(l);
	l->get_transform()->position = vector2f(m_tilesize * indices.x, m_tilesize * indices.y);
	l->get_tex_draw()->set_width_height(m_tilesize, m_tilesize);
}

void level_grid::set_score_text(int pts)
{
	std::stringstream ss;
	ss << "SCORE: " << pts;
	engine::get_instance()->get_renderer()->bottom_text = ss.str();
}

void level_grid::kill_disposable_enemies()
{
	for (enemy *e : enemies_to_kill)
	{
		std::vector<enemy *>::iterator looking_for_instance = std::find(enemies.begin(), enemies.end(), e);
		if (looking_for_instance != enemies.end())
		{
			engine::get_instance()->get_scene()->remove_actor(*looking_for_instance);
			delete *looking_for_instance;
			enemies.erase(looking_for_instance);

			get_hero()->add_killer_points(120);

			set_score_text(get_hero()->get_total_points());

			//put_enemy_on_tile(get_respawner_tile());
			call_respawn();
		}
	}
}

tile* level_grid::get_respawner_tile()
{
	return get(spawner.x, spawner.y);
}

void level_grid::call_respawn()
{
	float respawn_moment = engine::get_time_from_start() + m_enemyRespawnTime;
	if (!m_respawnMoments.empty() 
		&& (respawn_moment - m_respawnMoments.back() < m_minimumTimeBetweenEnemyRespawns))
		respawn_moment = m_respawnMoments.back() + m_minimumTimeBetweenEnemyRespawns;

	m_respawnMoments.push_back(respawn_moment);
}

void level_grid::handle_respawns()
{
	if (!m_respawnMoments.empty())
	{
		float respawn_moment = m_respawnMoments.front();
		if (respawn_moment < engine::get_time_from_start())
		{
			m_respawnMoments.pop_front();
			put_enemy_on_tile(get_respawner_tile());
		}
	}
}

level_grid::level_grid(file_reader_line_by_line *li, float tilesize, level_manager *manager)
{
	m_levelmanager = manager;

	m_tilesize = tilesize;

	int row = 0;
	int col = 0;
	
	for(std::string line: li->content)
	{
		tile_grid->push_back(new std::vector<tile*>());
		col = 0;
		for (char c : line)
		{
			bool add_gold = false;
			bool add_enemy = false;

			tile *tile = nullptr;

			switch(c)
			{
			case '#':
				tile = new wall(col, row, this);
				break;
			case '0':
				tile = new wall(col, row, this, false);
				break;
			case '=':
				tile = new ladder(col, row, this);
				break;
			case '-':
				tile = new pipe(col, row, this);
				break;
			case '@': // HERO SPAWN
				tile = new empty_tile(col, row, this);
				if (m_hero == nullptr)
				{
					m_hero = new hero(tile, this);
					engine::get_instance()->get_scene()->add_actor(m_hero);
				}
				break;
			case 'S': //ENEMY SPAWNER (RESPAWNER?)
				tile = new empty_tile(col, row, this);
				spawner = point(col, row);
				break;
			case '^': // PILE OF GOLD
				tile = new empty_tile(col, row, this);
				add_gold = true;
				break;
			case 'X': // ENEMY
				tile = new empty_tile(col, row, this);
				add_enemy = true;
				break;
			case '!':
				tile = new empty_tile(col, row, this);
				hidden_ladders.push_back({ col, row });
				break;
			case '?':
				tile = new empty_tile(col, row, this);
				final_ladder = { col,row };
				break;
			default:
				tile = new empty_tile(col, row, this);
			}

			tile->get_transform()->position = vector2f(tilesize * col, tilesize * row);
			tile->get_tex_draw()->set_width_height(tilesize, tilesize);
			engine::get_instance()->get_scene()->add_actor(tile);

			tile_grid->at(row)->push_back(tile);
			if (add_gold)
			{
				put_gold_on_tile(tile, 100);
			} else if (add_enemy)
			{
				put_enemy_on_tile(tile);
			}
			++col;
		}

		++row;
	}

	required_gold = 0;
	for (gold *g : gold_piles)
		required_gold += g->get_value();

	set_score_text(0);
}

level_grid::~level_grid()
{
	during_deconstruction = true;
	//REMOVE TILES
	for (int i = 0;i < tile_grid->size();i++)
	{
		for (std::vector<tile *>::iterator it = tile_grid->at(i)->begin(); it != tile_grid->at(i)->end(); ++it) {
			engine::get_instance()->get_scene()->remove_actor(*it);
			delete *it;
		}
		tile_grid->at(i)->clear();
	}

	//REMOVE GOLD

	for (std::vector<gold *>::iterator it = gold_piles.begin(); it != gold_piles.end(); ++it) {
			engine::get_instance()->get_scene()->remove_actor(*it);
			delete *it;
	}

	//REMOVE ENEMIES
	for (std::vector<enemy *>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
		engine::get_instance()->get_scene()->remove_actor(*it);
		delete *it;
	}

	engine::get_instance()->get_scene()->remove_actor(m_hero);
	delete m_hero;
}

tile* level_grid::get(int column, int row)
{
	if (row >= tile_grid->size()) return nullptr;
	if (column >= tile_grid->at(row)->size()) return nullptr;

	return tile_grid->at(row)->at(column);
}

void level_grid::put_gold_on_tile(tile* tile, int value)
{
	gold *go = new gold(value);
	if (go->set_tile(tile, this))
	{
		engine::get_instance()->get_scene()->add_actor(go);
		
		
		gold_piles.push_back(go);
	}
	else
		delete go;
}

void level_grid::put_enemy_on_tile(tile* tile)
{
	enemy *enem = new enemy(tile, this);
	engine::get_instance()->get_scene()->add_actor(enem);
	enemies.push_back(enem);
}

int level_grid::get_required_gold()
{
	return required_gold;
}

bool level_grid::on_hero_gold_take(int golden_points, int killer_points)
{
	set_score_text(golden_points + killer_points);

	if (golden_points == required_gold)
	{
		std::cout << "UNLOCK\n";
	
		//CHANGING SOME RANDOM TILE, TO BE REPLACED LATER
		for (point p : hidden_ladders)
			unhide_ladder(p, false);
	
		unhide_ladder(final_ladder, true);

		m_hero->reload_key_tiles();

		for (enemy *e : enemies)
			e->reload_key_tiles();

		return true;
	}

	return false;
}

void level_grid::on_gold_disappearance(gold *g)
{
	if (!during_deconstruction)
	{
		std::vector<gold *>::iterator looking_for_instance = std::find(gold_piles.begin(), gold_piles.end(), g);
		if (looking_for_instance != gold_piles.end())
		{
			gold_piles.erase(looking_for_instance);
		}
	}
}

void level_grid::on_enemy_death(enemy* e)
{
	enemies_to_kill.push_back(e);
		
}

void level_grid::update()
{
	kill_disposable_enemies();
	handle_respawns();
}
