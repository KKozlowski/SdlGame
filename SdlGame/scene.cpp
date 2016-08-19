#include "scene.h"
#include "actor.h"

scene::scene()
{
	actors = new std::set<actor *>();
	to_add = new std::set<actor *>();
	to_delete = new std::set<actor *>();
}

void scene::update()
{
	for (actor *a : *to_delete)
	{
		if (actors->find(a) != actors->end())
		{
			actors->erase(a);
			delete a;
		}

	}
	for (actor *a : *to_add)
		actors->insert(a);

	to_add->clear();
	to_delete->clear();

	for (actor *a : *actors)
		if (to_delete->find(a) == to_delete->end())
			a->update();

	

	

	
}

bool scene::add_actor(actor* a)
{
	to_add->insert(a);
	return true;
}

bool scene::remove_actor(actor* a)
{
	to_delete->insert(a);
	return true;
}

void scene::remove_actors(std::initializer_list<actor*> a)
{
	for (actor *ac : a)
		remove_actor(ac);
}
