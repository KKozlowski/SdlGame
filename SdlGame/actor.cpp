#include "actor.h"

actor::actor()
{
	transform_ = new transform(this);
}

void actor::update()
{
}

draw_base* actor::get_draw() const
{
	return drawing;
}

