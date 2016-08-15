#include "level_grid.h"
#include <fstream>
#include <iostream>
#include "ball.h"
#include "engine.h"
#include "scene.h"
#include "draw_texture.h"
#include "pipe.h"
#include "gold.h"
#include "level_manager.h"

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
			std::cout << c;
			tile *tile = nullptr;

			switch(c)
			{
			case '#':
				tile = new wall(col, row, this);
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
			case '^': // PILE OF GOLD
				tile = new empty_tile(col, row, this);
				add_gold = true;
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
			}
			++col;
		}

		std::cout << std::endl;
		++row;
	}
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

void level_grid::at_gold_disappearance(gold *g)
{
	if (!during_deconstruction)
	{
		std::vector<gold *>::iterator looking_for_instance = std::find(gold_piles.begin(), gold_piles.end(), g);
		if (looking_for_instance != gold_piles.end())
			gold_piles.erase(looking_for_instance);

		if (gold::get_existing_count() == 0)
		{
			std::cout << "UNLOCK\n";

			//CHANGING SOME RANDOM TILE, TO BE REPLACED LATER
			for (point p : hidden_ladders)
				unhide_ladder(p, false);

			unhide_ladder(final_ladder, true);
		}
	}
	
}
