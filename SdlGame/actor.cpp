#include "actor.h"
#include <iostream>

actor::actor()
{
	transform_ = new transform(this);
	point *p = new point();
	delete p;
}

actor::~actor()
{
	delete transform_;
	delete drawing;
}

void actor::update()
{
}

draw_base* actor::get_draw() const
{
	return drawing;
}

