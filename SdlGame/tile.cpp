#include "tile.h"

tile::tile(int x, int y, level_grid* l)
{
	indices = point(x, y);
	m_level = l;
	drawing = new draw_texture(this, "");
	get_tex_draw()->centered = true;
	get_tex_draw()->set_depth(10);
}
