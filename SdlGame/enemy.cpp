#include "enemy.h"
#include "draw_texture.h"
#include "tile.h"
#include "level_grid.h"

void enemy::set_current_tile(tile* t)
{
	current_tile = t;
}

point enemy::get_2d_distance_to_tile(tile* t)
{
	return point(
		t->get_Xpos() - current_tile->get_Xpos(), 
		t->get_Ypos() - current_tile->get_Ypos()
		);
}

enemy::enemy(tile* start_tile, level_grid* lg)
{
	m_levelgrid = lg;
	draw_texture *dt = new draw_texture(this, "enemy.png");
	dt->set_width_height(lg->get_tilesize(), lg->get_tilesize());
	dt->centered = true;
	drawing = dt;

	set_current_tile(start_tile);
	get_transform()->position = start_tile->get_transform()->position;

	m_speed = 2.f;
}

void enemy::update()
{
	hero *he = m_levelgrid->get_hero();
	if ((get_transform()->position - he->get_transform()->position).length() < killing_range*m_levelgrid->get_tilesize())
	{
		he->die();
	}

	tile *hero_tile = he->get_current_tile();
}
