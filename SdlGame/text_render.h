#pragma once
#include "actor.h"

class text_render : public actor
{
public:
	std::string content;
	point position;
	int height;

	text_render();
	text_render(std::string text, point start_point, int size);
};
