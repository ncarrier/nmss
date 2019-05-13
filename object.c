#include <stdlib.h>
#include <error.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "object.h"
#include "screen.h"

static void sdl_surface_cleanup(struct SDL_Surface **surface) {
	if (surface == NULL || *surface == NULL)
		return;

	SDL_FreeSurface(*surface);
	*surface = NULL;
}

void object_init(struct object *object, struct SDL_Renderer *renderer,
		const struct SDL_Rect *rect, const char *image) {
	struct SDL_Surface __attribute__((cleanup(sdl_surface_cleanup)))*surface = NULL;

	object->renderer = renderer;
	if (rect != NULL)
		object->dst = *rect;
	object->flip = SDL_FLIP_NONE;
	surface = IMG_Load(image);
	if (surface == NULL)
		error(EXIT_FAILURE, 0, "IMG_Load: %s", SDL_GetError());

	object->texture = SDL_CreateTextureFromSurface(object->renderer, surface);
	if (object->texture == NULL)
		error(EXIT_FAILURE, 0, "SDL_CreateTexture: %s", SDL_GetError());
	object->dst.w = SCREEN_SPRITE_WIDTH;
	object->dst.h = SCREEN_SPRITE_HEIGHT;
}

void object_set_pos(struct object *object, const struct SDL_Rect *rect) {
	object->dst.x = rect->x;
	object->dst.y = rect->y;
}

void object_render(struct object *object) {
	SDL_RenderCopyEx(object->renderer, object->texture, NULL, &object->dst,
			0., NULL, object->flip);
}

void object_toggle_flip(struct object *object, SDL_RendererFlip flip) {
	object->flip ^= flip;
}

void object_cleanup(struct object *object) {
	if (object == NULL)
		return;

	if (object->texture != NULL)
		SDL_DestroyTexture(object->texture);
}
