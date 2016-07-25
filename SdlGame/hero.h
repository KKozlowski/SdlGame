#pragma once
#include "ball.h"
#include "rigidbody.h"

class hero : public ball
{
protected:
	float move_speed = 210;
	rigidbody *m_rigidbody;
public:
	hero();
	~hero();
	virtual void update() override;
};
