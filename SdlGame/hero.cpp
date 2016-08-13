#include "hero.h"
#include "input.h"
#include "engine.h"
#include "camera.h"
#include "draw_texture.h"

#include "tile.h"
#include "level_grid.h"
#include <iostream>

hero::hero(tile *start_tile, level_grid *lg)
{
	current_tile = start_tile;
	float tilesize = lg->get_tilesize();

	static_cast<draw_texture *>(get_draw())->set_width_height(tilesize, tilesize);

	get_transform()->position = vector2f(start_tile->get_Xpos()*tilesize, start_tile->get_Ypos()*tilesize);

	get_draw()->set_depth(-10);
}

hero::~hero()
{

}

void hero::update()
{
	input *inp = engine::get_instance()->get_input();
	//get_transform()->position.x += engine::get_delta_time() * 100;

	if (inp->get_key(SDLK_w))
		get_transform()->position -= vector2f(0, engine::get_delta_time()*move_speed);
	if (inp->get_key(SDLK_s))
		get_transform()->position += vector2f(0, engine::get_delta_time()*move_speed);
	if (inp->get_key(SDLK_a))
		get_transform()->position -= vector2f(engine::get_delta_time()*move_speed,0);
	if (inp->get_key(SDLK_d))
		get_transform()->position += vector2f(engine::get_delta_time()*move_speed, 0);

	engine::get_instance()->get_renderer()->get_camera()->center = point(get_transform()->position.x, get_transform()->position.y);
}
