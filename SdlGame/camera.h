#pragma once
#include "vector2.h"
#include "renderer.h"

class camera
{
private:
	renderer *m_renderer;
	
public:
	point center;
	float view_radius = 360.f;

	camera(point center, float view_radius, renderer *rend)
	{
		this->center = center;
		this->view_radius = view_radius;
		m_renderer = rend;
	}

	float get_scale()
	{
		return  (m_renderer->SCREEN_HEIGHT / 2)/ view_radius;
	}

	SDL_Rect translate_rect(SDL_Rect origin)
	{
		float scal = get_scale();

		origin.w *= scal;
		origin.h *= scal;

		origin.x = center.x + (origin.x - center.x)*scal + m_renderer->SCREEN_WIDTH/2 - center.x;
		origin.y = center.y + (origin.y - center.y)*scal + m_renderer->SCREEN_HEIGHT / 2 - center.y;

		return origin;
	}
};
