#pragma once
#include "actor.h"

class actor_component
{
protected:
	actor *m_Actor;
public:
	actor_component(actor *a);
	actor * get_actor() const;
	virtual void update() {}
	
};