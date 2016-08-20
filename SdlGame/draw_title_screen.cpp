#include "draw_title_screen.h"
#include "engine.h"
#include "text_render.h"
#include "scene.h"

draw_title_screen::draw_title_screen(actor* a)
	:draw_base(a)
{
	engine::get_instance()->get_scene()->add_actor(new text_render("NOM", point(), 120));
}

void draw_title_screen::draw(SDL_Renderer* renderer, camera* cam)
{
	//SOME HEAVY HACKINH HERE!!!11
	engine::get_instance()->get_renderer()->print_text("LODE RUNNER", point(100, 160), 96);
	engine::get_instance()->get_renderer()->print_text("on SDL", point(110, 256), 32);

	engine::get_instance()->get_renderer()->print_text("WSAD - movement", point(110, 480), 32);
	engine::get_instance()->get_renderer()->print_text("QE   - digging", point(110, 516), 32);
	engine::get_instance()->get_renderer()->print_text("Esc  - exit", point(110, 552), 32);
}
