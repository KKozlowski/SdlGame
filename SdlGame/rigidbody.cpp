#include "rigidbody.h"
#include "engine.h"

rigidbody::rigidbody(actor* a) : actor_component(a)
{
}

void rigidbody::update()
{
	//velocity -= velocity.normalized() * drag * engine::get_delta_time();
	vector2f normalized = velocity.normalized();
	if (abs(velocity.x) < 0.1f) 
		velocity.x = 0;
	else
	{
		float start = velocity.x;
		velocity.x -= velocity.x * (velocity.x > 2 ? drag : 3*drag) * engine::get_delta_time();
		if (velocity.x * start < 0) velocity.x = 0;
	}
		

	if (abs(velocity.y) < 0.01f)
		velocity.y = 0;
	else
	{
		float start = velocity.y;
		velocity.y -= velocity.y * (velocity.y > 2 ? drag : 3 * drag) * engine::get_delta_time();
		if (velocity.y * start < 0) velocity.y = 0;
	}
		
	//velocity.cut_negative();
	get_actor()->get_transform()->position = get_actor()->get_transform()->position + velocity * engine::get_delta_time();
}
