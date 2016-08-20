#include "hero_input.h"
#include "input.h"
#include "engine.h"

hero_input::hero_input(hero* h, input *i)
	: m_hero(h), m_input(i)
{

}

void hero_input::update()
{

	if (m_input->get_key(dig_left_key))
		m_hero->dig({ -1,0 });
	else if (m_input->get_key(dig_right_key))
		m_hero->dig({ 1,0 });
	else if (m_input->get_key(up_key))
	{
		m_hero->move_up();
	}
	else if (m_input->get_key(down_key))
	{
		m_hero->move_down();
	}
	else if (m_input->get_key(left_key))
	{
		m_hero->move_side(-1);
	}
	else if (m_input->get_key(right_key))
	{
		m_hero->move_side(1);
	}
	else
		m_hero->move_idle();
}
