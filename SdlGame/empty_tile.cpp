#include "empty_tile.h"

empty_tile::empty_tile(int x, int y, level_grid* l)
	:tile(x,y,l)
{
	m_type = tile_type::empty;
}
