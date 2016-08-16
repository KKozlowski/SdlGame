#pragma once
#include "vector2.h"
#include "tile.h"

class level_grid;

class tile_traveller : public actor
{
protected:
	level_grid *m_levelgrid = nullptr;

	tile *m_currentTile = nullptr;
	tile *m_destinationTile = nullptr;

	point m_currentTileIndices;
	point m_destinationTileIndices;

	point m_previousDirection;
	float m_movementProgress = 0;

	float m_movementSpeed = 0;

	virtual void set_current_tile(tile *t);
	virtual void set_destination_tile(tile *t);
public:
	tile_traveller(level_grid *lg, float speed);
	virtual ~tile_traveller();

	///<summary>
	/// Reloads key tiles (current and destination) right from level grid,
	/// according to indices saved before.
	/// <para/>Should be called right after major changes in levelgrid.
	/// <para/>For example, after showing the final ladder, traveller can be
	/// on one of destroyed tiles!
	///</summary>
	void reload_key_tiles();
};
