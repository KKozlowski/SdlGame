#include "level_grid.h"
#include <fstream>
#include <iostream>
#include "ball.h"
#include "engine.h"
#include "scene.h"
#include "draw_texture.h"
#include "pipe.h"
#include "gold.h"

level_grid::level_grid(std::string filename, float tilesize, point start)
{
	std::ifstream read(filename);
	std::string line;

	m_tilesize = tilesize;

	int row = 0;
	int col = 0;
	
	while (std::getline(read, line))
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
			case '@':
				tile = new empty_tile(col, row, this);
				if (m_hero == nullptr)
				{
					m_hero = new hero(tile, this);
					engine::get_instance()->get_scene()->add_actor(m_hero);
				}
				break;
			case '^':
				tile = new empty_tile(col, row, this);
				add_gold = true;
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
				gold *go = new gold(100);
				if (go->set_tile(tile, this))
				{
					engine::get_instance()->get_scene()->add_actor(go);
				}
				else
					delete go;
			}
			++col;
		}

		std::cout << std::endl;
		++row;
	}
}

tile* level_grid::get(int column, int row)
{
	if (row >= tile_grid->size()) return nullptr;
	if (column >= tile_grid->at(row)->size()) return nullptr;

	return tile_grid->at(row)->at(column);
}

void level_grid::at_gold_disappearance()
{
	if (gold::get_existing_count() == 0)
	{
		std::cout << "UNLOCK\n";
	}
}
