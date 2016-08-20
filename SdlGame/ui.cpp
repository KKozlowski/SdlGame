#include "ui.h"
#include "engine.h"
#include "scene.h"

ui::ui()
{
	m_titleScreen = new title_screen();
	engine::get_instance()->get_scene()->add_actor(m_titleScreen);

	m_bottomText = new text_render("", point(10, engine::get_instance()->get_renderer()->SCREEN_HEIGHT - 50), 50);
	engine::get_instance()->get_scene()->add_actor(m_bottomText);
}

ui::~ui()
{
}

bool ui::hide_title_screen()
{
	if (m_titleScreen != nullptr)
	{
		engine::get_instance()->get_scene()->remove_actor(m_titleScreen);
		m_titleScreen = nullptr;
		return true;
	}
	
	return false;
}
