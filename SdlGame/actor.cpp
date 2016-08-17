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
	if (m_draw != nullptr)
	{
		delete m_draw;
		m_draw = nullptr;
	}

}

void actor::update()
{

}



