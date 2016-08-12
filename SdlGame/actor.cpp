#include "actor.h"
#include <iostream>
#include "draw_base.h"

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



