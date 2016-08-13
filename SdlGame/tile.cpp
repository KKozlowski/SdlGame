#include "tile.h"

tile::tile(int x, int y, level_grid* l)
{
	indices = point(x, y);
	m_level = l;
}
