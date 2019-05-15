#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "error.h"

#include <SDL2/SDL.h>

#include "input.h"
#include "game.h"

static void sdl_window_cleanup(struct SDL_Window **window) {
	if (window == NULL || *window == NULL)
		return;

	SDL_DestroyWindow(*window);
	*window = NULL;
}

static void sdl_renderer_cleanup(struct SDL_Renderer **renderer) {
	if (renderer == NULL || *renderer == NULL)
		return;

	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;
}

int main(int argc, char *argv[])
{
	struct SDL_Window __attribute__((cleanup(sdl_window_cleanup)))*window = NULL;
	struct SDL_Renderer __attribute__((cleanup(sdl_renderer_cleanup)))*renderer = NULL;
	int ret;
	struct game __attribute__((cleanup(game_cleanup))) game;

	ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	if (ret != 0)
		error(EXIT_FAILURE, 0, "SDL_Init: %s", SDL_GetError());
	atexit(SDL_Quit);
	window = SDL_CreateWindow("nmss", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 256, 64, SDL_WINDOW_SHOWN);
	if (window == NULL)
		error(EXIT_FAILURE, 0, "SDL_CreateWindow: %s", SDL_GetError());
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
		error(EXIT_FAILURE, 0, "SDL_CreateRenderer: %s", SDL_GetError());

	game_init(&game, renderer);
	while (game.input.loop) {
		SDL_RenderClear(renderer);
		game_update(&game);
		SDL_RenderPresent(renderer);
	}

	return 0;
}

