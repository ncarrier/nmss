#include <SDL2/SDL_image.h>

#include "score.h"
#include "utils.h"
#include "error.h"
#include "res/numbers.xpm"

static const struct SDL_Rect digit_size = { .h = 5, .w = 6 };

void score_init(struct score *score, struct SDL_Renderer *renderer)
{
	struct SDL_Surface __attribute__((cleanup(sdl_surface_cleanup)))*surface = NULL;

	score->value = 0;
	score->renderer = renderer;
	surface = IMG_ReadXPMFromArray(numbers_xpm);
	if (surface == NULL)
		error(EXIT_FAILURE, 0, "IMG_Load: %s", SDL_GetError());

	score->texture = SDL_CreateTextureFromSurface(score->renderer, surface);
	if (score->texture == NULL)
		error(EXIT_FAILURE, 0, "SDL_CreateTexture: %s", SDL_GetError());
}

void score_update(struct score *score)
{
	struct SDL_Rect pos;
	struct SDL_Rect sub;
	unsigned value;
	unsigned digit;

	sub = digit_size;
	pos = digit_size;
	pos.y = 56;
	pos.x = 247;

	for (value = score->value; value != 0; value /= 10) {
		digit = value % 10;
		sub.x = digit * 6;
		SDL_RenderCopy(score->renderer, score->texture, &sub, &pos);
		pos.x -= 6;
	}
}

void score_increase(struct score *score)
{
	score->value++;
}

void score_cleanup(struct score *score)
{
	if (score == NULL)
		return;

	if (score->texture != NULL)
		SDL_DestroyTexture(score->texture);
}
