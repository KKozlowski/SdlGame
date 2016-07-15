#include "scene.h"
#include "actor.h"

scene::scene()
{
	actors = new std::set<actor *>();
}

void scene::update()
{
	for (actor *a : *actors)
		a->update();
}

bool scene::add_actor(actor* a)
{
	actors->insert(a);
	return true;
}

bool scene::remove_actor(actor* a)
{
	actors->erase(a);
	return true;
}
