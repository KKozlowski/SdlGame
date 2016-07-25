﻿#include "hero.h"
#include "input.h"
#include "engine.h"

hero::hero()
{
	m_rigidbody = new rigidbody(this);
	m_rigidbody->drag = 0.7f;
}

hero::~hero()
{
	delete m_rigidbody;
}

void hero::update()
{
	input *inp = engine::get_instance()->get_input();
	//get_transform()->position.x += engine::get_delta_time() * 100;

	if (inp->get_key(SDLK_w))
		m_rigidbody->velocity -= vector2f(0, engine::get_delta_time()*move_speed);
	if (inp->get_key(SDLK_s))
		m_rigidbody->velocity += vector2f(0, engine::get_delta_time()*move_speed);
	if (inp->get_key(SDLK_a))
		m_rigidbody->velocity -= vector2f(engine::get_delta_time()*move_speed,0);
	if (inp->get_key(SDLK_d))
		m_rigidbody->velocity += vector2f(engine::get_delta_time()*move_speed, 0);

	m_rigidbody->update();
}
