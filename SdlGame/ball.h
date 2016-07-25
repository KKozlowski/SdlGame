#pragma once
#include "actor.h"

class ball : public actor
{
public:
	ball();
	virtual ~ball();
	virtual void update() override;
};
