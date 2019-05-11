#include <stdlib.h>
#include <stdio.h>

#include <error.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static void sdl_window_cleanup(SDL_Window **window) {
	if (window == NULL || *window == NULL)
		return;

	SDL_DestroyWindow(*window);
	*window = NULL;
}

static void sdl_renderer_cleanup(SDL_Renderer **renderer) {
	if (renderer == NULL || *renderer == NULL)
		return;

	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;
}

static void sdl_texture_cleanup(SDL_Texture **texture) {
	if (texture == NULL || *texture == NULL)
		return;

	SDL_DestroyTexture(*texture);
	*texture = NULL;
}

static void sdl_surface_cleanup(SDL_Surface **surface) {
	if (surface == NULL || *surface == NULL)
		return;

	SDL_FreeSurface(*surface);
	*surface = NULL;
}

int main(int argc, char *argv[])
{
	SDL_Window __attribute__((cleanup(sdl_window_cleanup)))*window = NULL;
	SDL_Renderer __attribute__((cleanup(sdl_renderer_cleanup)))*renderer = NULL;
	SDL_Texture __attribute__((cleanup(sdl_texture_cleanup)))*texture = NULL;
	SDL_Surface __attribute__((cleanup(sdl_surface_cleanup)))*surface = NULL;
	int ret;

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret != 0)
		error(EXIT_FAILURE, 0, "SDL_Init: %s", SDL_GetError());
	atexit(SDL_Quit);
	window = SDL_CreateWindow("nmss", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 256, 64, SDL_WINDOW_SHOWN);
	if (window == NULL)
		error(EXIT_FAILURE, 0, "SDL_CreateWindow: %s", SDL_GetError());
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		error(EXIT_FAILURE, 0, "SDL_CreateRenderer: %s", SDL_GetError());
	SDL_RenderClear(renderer);
	surface = IMG_Load("res/ship.png");
	if (surface == NULL)
		error(EXIT_FAILURE, 0, "IMG_Load: %s", SDL_GetError());

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL)
		error(EXIT_FAILURE, 0, "SDL_CreateTexture: %s", SDL_GetError());

	SDL_Rect dst = {10, 10, 8, 8};
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_RenderPresent(renderer);

	SDL_Delay(1000);

	return 0;
}

