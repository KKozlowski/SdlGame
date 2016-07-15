#include "draw_texture.h"
#include "actor.h"
draw_texture::draw_texture(actor* a, std::string filename)
	: draw_base(a)
{
	thatTexture = texture::get_texture(filename);
	if (thatTexture == nullptr)
	{
		printf("Failed to load texture image!\n");
	}

	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = 120;
	stretchRect.h = 120;
}

void draw_texture::draw(SDL_Renderer *renderer)
{
	draw_base::draw(renderer);

	SDL_Rect *transformRekt = lActor->get_transform()->get_rekt();

	SDL_Rect sum = stretchRect;
	sum.x += transformRekt->x;
	sum.y += transformRekt->y;

	if (centered)
	{
		sum.x -= sum.w / 2;
		sum.y -= sum.h / 2;
	}

	SDL_RenderCopy(renderer, thatTexture, nullptr, &sum);
	//Stuff
}
