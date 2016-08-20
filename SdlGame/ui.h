#pragma once
#include "text_render.h"
#include "title_screen.h"

class ui
{
	///<summary>Text that is displayed at the bottom of the screen. Good for score displaying. </summary>
	text_render *m_bottomText = nullptr;

	title_screen *m_titleScreen = nullptr;
public:
	ui();
	~ui();

	void set_bottom_text(std::string content) { m_bottomText->content = content; }
	bool hide_title_screen();
};
