#pragma once
#include <cstdint>
#include <SDL.h>
#include "transform.h"

class camera;
class actor;

class draw_base
{
private:
	bool visible = true;
protected:
	int16_t layer;
	actor *m_actor;
public:
	draw_base(actor *a);
	virtual ~draw_base();

	///<summary>
	///Gets rendering depth of the object. 
	///The higher the number, the farther drawing plane.
	///</summary>
	inline int16_t get_depth() const;

	///<summary>
	///Gets rendering depth of the object. 
	///The higher the number, the farther drawing plane.
	///</summary>
	inline void set_depth(int16_t new_layer);

	inline actor *get_actor() const;

	void set_visible(bool visible) { this->visible = visible; }
	bool is_visible() const { return visible; }

	virtual void draw(SDL_Renderer *renderer, camera *cam);
};

int16_t draw_base::get_depth() const
{
	return layer;
}

void draw_base::set_depth(int16_t new_layer)
{
	layer = new_layer;
}

actor* draw_base::get_actor() const
{
	return m_actor;
}