//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "engine.h"

//Screen dimension constants



int main(int argc, char* args[])
{
	engine::get_instance()->run();

	return 0;
}