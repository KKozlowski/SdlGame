#include "ball.h"
#include "draw_texture.h"
#include "engine.h"

ball::ball()
{
	draw_texture *dt = new draw_texture(this, "texture.png");
	dt->centered = true;
	drawing = dt;
}

ball::~ball()
{
}

void ball::update()
{
	//get_transform()->position.x += engine::get_delta_time()*100;
	//printf("%s\n", get_transform()->position.to_string().c_str());
}
