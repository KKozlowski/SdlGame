#pragma once
#include "actor.h"

class hero;
class input;

class hero_input : public actor
{
private:
	hero *m_hero;
	input *m_input;
public:
	hero_input(hero *h, input *i);
	void update() override;
};
