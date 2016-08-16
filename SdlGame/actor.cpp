#include "actor.h"
#include <iostream>
#include "draw_base.h"
#include "engine.h"

actor::actor()
{
	m_transform = new transform(this);
}

actor::~actor()
{
	delete m_transform;
	engine::get_instance()->get_renderer()->remove_draw(m_draw);
}

void actor::update()
{

}



