#pragma once
#include "vector2.h"
#include "renderer.h"

class camera
{
private:
	renderer *m_renderer;
	
public:
	///<summary>Center of the area camera "sees".</summary>
	point center;

	///<summary>Amount of units "seen" by camera from the center of the view to the top/bottom edge of the view.</summary>
	float view_radius = 360.f;

	camera(point center, float view_radius, renderer *rend)
	{
		this->center = center;
		this->view_radius = view_radius;
		m_renderer = rend;
	}

	float get_scale() const
	{
		return  (m_renderer->SCREEN_HEIGHT / 2)/ view_radius;
	}

	///<summary>Transforms given rect (origin) from world space to view space.</summary>
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
