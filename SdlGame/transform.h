#pragma once
#include "vector2.h"
#include <SDL.h>
class actor;

class transform
{
private:
	actor *lActor;
	SDL_Rect *rekt;
public:
	explicit transform(actor *a);

	vector2<float> position;
	vector2<float> scale;
	float rotation;

	SDL_Rect *get_rekt();
	
	actor *get_actor() const;
};


