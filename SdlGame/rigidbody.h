#pragma once
#include "actor_component.h"
#include "vector2.h"

class rigidbody : public actor_component
{
private:
	
public:
	vector2f velocity;
	float drag = 0.f;

	rigidbody(actor *a);
	virtual void update() override;
};
