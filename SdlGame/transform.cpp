#include "transform.h"
#include "math.h"

actor* transform::get_actor() const
{
	return m_actor;
}

transform::transform(actor* a)
{
	m_actor = a;
	m_rekt = new SDL_Rect();
}

SDL_Rect* transform::get_rekt()
{
	m_rekt->x = floor(position.x);
	m_rekt->y = floor(position.y);
	m_rekt->w = 0;
	m_rekt->h = 0;
	return m_rekt;
}
