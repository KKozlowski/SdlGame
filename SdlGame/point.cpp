#include "point.h"


point::point(uint32_t x, uint32_t y)
{
	this->x = x;
	this->y = y;
}

point::point()
	:point(0,0)
{
}
