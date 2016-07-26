#pragma once
#include <set>

class actor;
class scene
{
private:
	std::set<actor *> *actors;
public:
	scene();

	///<summary>
	///It calls update() method on all the gathered actors.
	///</summary>
	void update();

	bool add_actor(actor *a);
	bool remove_actor(actor *a);
	void remove_actors(std::initializer_list<actor *> a);
};
