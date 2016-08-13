#include "ladder.h"

ladder::ladder(int x, int y, level_grid* l)
	:tile(x,y,l)
{
	m_type = tile_type::ladder;
	get_tex_draw()->set_texture("ladder.png");
}
