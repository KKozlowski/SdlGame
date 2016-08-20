#pragma once
#include "draw_base.h"
#include "transform.h"
#include "updateable.h"

///<summary>
/// Basic class of an active object, that contains transform data,
/// and is updated every frame. To become active, it needs to be added
/// to scene object.
///</summary>
class actor : public updateable
{
private:
	transform *m_transform;
protected:
	draw_base *m_draw;
public:
	actor();
	virtual ~actor();

	///<summary> Called every frame. In this method, actor's state should be modified. </summary>
	virtual void update() override;

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
