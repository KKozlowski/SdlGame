#pragma once
#include "vector2.h"
#include <SDL.h>
class actor;

class transform
{
private:
	actor *m_actor;
	SDL_Rect *m_rekt;
public:
	explicit transform(actor *a);

	vector2f position;
	//vector2f scale;
	//float rotation;

	///<summary>
	///Returns an SDL_Rect object with zero width/height, but a proper x/y position.
	///</summary>
	SDL_Rect *get_rekt();
	
	actor *get_actor() const;
};


