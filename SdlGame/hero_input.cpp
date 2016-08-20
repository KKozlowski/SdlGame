#include "hero_input.h"
#include "input.h"
#include "engine.h"

hero_input::hero_input(hero* h, input *i)
	: m_hero(h), m_input(i)
{

}

void hero_input::update()
{

	if (m_input->get_key(SDLK_q))
		m_hero->dig({ -1,0 });
	else if (m_input->get_key(SDLK_e))
		m_hero->dig({ 1,0 });
	else if (m_input->get_key(SDLK_w))
	{
		m_hero->move_up();
	}
	else if (m_input->get_key(SDLK_s))
	{
		m_hero->move_down();
	}
	else if (m_input->get_key(SDLK_a))
	{
		m_hero->move_side(-1);
	}
	else if (m_input->get_key(SDLK_d))
	{
		m_hero->move_side(1);
	}
	else
		m_hero->move_idle();
}
