#pragma once
#include <cstdint>
#include <SDL.h>
#include "transform.h"

class actor;

class draw_base
{
protected:
	int16_t layer;
	actor *lActor;
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

	virtual void draw(SDL_Renderer *renderer);
};
