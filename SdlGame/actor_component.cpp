#include "actor.h"
#include "actor_component.h"

actor_component::actor_component(actor* a)
{
	m_Actor = a;
}

actor* actor_component::get_actor() const
{
	return m_Actor;
}
