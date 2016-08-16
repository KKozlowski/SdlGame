#include "tile_traveller.h"
#include "level_grid.h"

void tile_traveller::set_current_tile(tile* t)
{
	m_currentTile = t;
	m_currentTileIndices = m_currentTile->get_indices();
}

void tile_traveller::set_destination_tile(tile* t)
{
	m_destinationTile = t;
	m_destinationTileIndices = m_destinationTile->get_indices();
}

tile_traveller::tile_traveller(level_grid* lg, float speed)
{
	m_levelgrid = lg;
	m_movementSpeed = speed;
}

tile_traveller::~tile_traveller()
{
}

void tile_traveller::reload_key_tiles()
{
	m_currentTile = m_levelgrid->get(m_currentTileIndices.x, m_currentTileIndices.y);
	m_destinationTile = m_levelgrid->get(m_destinationTileIndices.x, m_destinationTileIndices.y);
}
