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
	drawing->set_visible(false);
}

void actor::update()
{

}



