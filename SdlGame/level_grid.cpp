#include "level_grid.h"
#include <fstream>
#include <iostream>
#include "ball.h"
#include "engine.h"
#include "scene.h"
#include "draw_texture.h"

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
			if (c != ' ')
			{
				ball *b2 = new ball();
				b2->get_transform()->position = vector2f(tilesize * col, tilesize * row);
				static_cast<draw_texture *>(b2->get_draw())->set_width_height(tilesize, tilesize);
				engine::get_instance()->get_scene()->add_actor(b2);
			}
			++col;
		}
		std::cout << std::endl;
		++row;
	}
}
