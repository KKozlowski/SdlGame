#include "level_grid.h"
#include <fstream>
#include <iostream>
#include "ball.h"
#include "engine.h"
#include "scene.h"
#include "draw_texture.h"
#include "pipe.h"

level_grid::level_grid(std::string filename, float tilesize, point start)
{
	std::ifstream read(filename);
	std::string line;
	int row = 0;
	int col = 0;
	while (std::getline(read, line))
	{
		col = 0;
		for (char c : line)
		{
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
			default:
				tile = new empty_tile(col, row, this);
			}

			if (tile != nullptr) {
				tile->get_transform()->position = vector2f(tilesize * col, tilesize * row);
				tile->get_tex_draw()->set_width_height(tilesize, tilesize);
				engine::get_instance()->get_scene()->add_actor(tile);
			}
			++col;
		}
		std::cout << std::endl;
		++row;
	}
}
