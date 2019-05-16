#include <stdlib.h>

#include <SDL2/SDL.h>

#include "utils.h"

void sdl_surface_cleanup(struct SDL_Surface **surface) {
	if (surface == NULL || *surface == NULL)
		return;

	SDL_FreeSurface(*surface);
	*surface = NULL;
}

