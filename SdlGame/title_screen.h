#pragma once
#include "actor.h"
#include <vector>
#include "draw_text.h"

class title_screen : public actor
{
	std::vector<text_render *> texts;
public:
	title_screen();
	~title_screen();
};
