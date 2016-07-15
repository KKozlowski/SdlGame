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

	inline int16_t get_layer() const;
	inline void set_layer(int16_t new_layer);
	inline actor *get_actor() const;

	virtual void draw(SDL_Renderer *renderer);
};
