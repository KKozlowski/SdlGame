#include "pipe.h"

pipe::pipe(int x, int y, level_grid* l)
	:tile(x, y, l)
{
	m_type = tile_type::pipe;
	get_tex_draw()->set_texture("pipe.png");
}
