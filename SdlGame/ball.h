#pragma once
#include "actor.h"

class ball : public actor
{
public:
	ball();
	virtual void update() override;
};
