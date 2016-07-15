#pragma once
#include <set>

class actor;
class scene
{
private:
	std::set<actor *> *actors;
public:
	scene();

	void update();
	bool add_actor(actor *a);
	bool remove_actor(actor *a);
};
