#include "transform.h"
#include "math.h"

actor* transform::get_actor() const
{
	return lActor;
}

transform::transform(actor* a)
{
	lActor = a;
	rekt = new SDL_Rect();
}

SDL_Rect* transform::get_rekt()
{
	rekt->x = floor(position.x);
	rekt->y = floor(position.y);
	rekt->w = 0;
	rekt->h = 0;
	return rekt;
}
