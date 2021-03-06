#include <stdlib.h>
#include "error.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "object.h"
#include "screen.h"
#include "utils.h"

void object_init(struct object *object, struct SDL_Renderer *renderer,
		const struct SDL_Rect *rect, char *image[]) {
	struct SDL_Surface __attribute__((cleanup(sdl_surface_cleanup)))*surface = NULL;

	object->renderer = renderer;
	if (rect != NULL) {
		object->pos = *rect;
	} else {
		object->pos.w = SCREEN_SPRITE_WIDTH;
		object->pos.h = SCREEN_SPRITE_HEIGHT;
	}
	object->flip = SDL_FLIP_NONE;
	surface = IMG_ReadXPMFromArray(image);
	if (surface == NULL)
		error(EXIT_FAILURE, 0, "IMG_Load: %s", SDL_GetError());

	object->texture = SDL_CreateTextureFromSurface(object->renderer, surface);
	if (object->texture == NULL)
		error(EXIT_FAILURE, 0, "SDL_CreateTexture: %s", SDL_GetError());
}

void object_set_pos(struct object *object, const struct SDL_Rect *rect) {
	object->pos.x = rect->x;
	object->pos.y = rect->y;
}

void object_render(struct object *object) {
	SDL_RenderCopyEx(object->renderer, object->texture, NULL, &object->pos,
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
