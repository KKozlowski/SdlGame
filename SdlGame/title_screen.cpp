#include "title_screen.h"
#include "engine.h"
#include "scene.h"
#include "text_render.h"

title_screen::title_screen()
{

	texts.push_back(new text_render("LODE RUNNER", point(100, 160), 96));
	texts.push_back(new text_render("on SDL", point(110, 256), 32));

	texts.push_back(new text_render("WSAD - movement", point(110, 480), 32));
	texts.push_back(new text_render("QE   - digging", point(110, 516), 32));
	texts.push_back(new text_render("Esc  - exit", point(110, 552), 32));

	for (text_render *t : texts)
	{
		engine::get_instance()->get_scene()->add_actor(t);
	}
}

title_screen::~title_screen()
{
	
	for (actor *t : texts)
	{
		engine::get_instance()->get_scene()->remove_actor(t);
	}

	texts.clear();
}
