#include "draw_texture.h"
#include "actor.h"
#include "camera.h"

draw_texture::draw_texture(actor* a, std::string filename)
	: draw_base(a)
{
	if (filename != "")
		if (!set_texture(filename))
		{
			printf("Failed to load texture image!\n");
		}

	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = 120;
	stretchRect.h = 120;
}

void draw_texture::draw(SDL_Renderer *renderer, camera* cam)
{
	draw_base::draw(renderer, cam);

	SDL_Rect *transformRekt = lActor->get_transform()->get_rekt();

	SDL_Rect sum = stretchRect;
	sum.x += transformRekt->x;
	sum.y += transformRekt->y;

	if (centered)
	{
		sum.x -= sum.w / 2;
		sum.y -= sum.h / 2;
	}

	sum = cam->translate_rect(sum);

	SDL_RenderCopy(renderer, thatTexture, nullptr, &sum);
	//Stuff
}

bool draw_texture::set_width_height(int W, int H)
{
	if (W <= 0 || H <= 0)
		return false;

	stretchRect.w = W;
	stretchRect.h = H;
}

bool draw_texture::set_texture(std::string filename)
{
	SDL_Texture *tex = texture::get_texture(filename);
	if (tex == nullptr)
	{
		return false;
	}
	else
	{
		thatTexture = tex;
		return true;
	}
}
