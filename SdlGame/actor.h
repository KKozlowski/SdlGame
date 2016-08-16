#pragma once
#include "draw_base.h"
#include "transform.h"
class actor
{
private:
	transform *m_transform;
protected:
	draw_base *m_draw;
public:
	actor();
	virtual ~actor();
	///<summary>
	///Called every frame. In this method, actor's state should be modified.
	///</summary>
	virtual void update();

	inline draw_base *get_draw() const;
	inline transform *get_transform() const;
};

transform* actor::get_transform() const
{
	return m_transform;
}

draw_base* actor::get_draw() const
{
	return m_draw;
}
