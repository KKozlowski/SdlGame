#include "actor.h"
#include <iostream>

actor::actor()
{
	m_transform = new transform(this);
	point *p = new point();
	delete p;
}

actor::~actor()
{
	delete m_transform;
	delete drawing;
}

void actor::update()
{
}

draw_base* actor::get_draw() const
{
	return drawing;
}

