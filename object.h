#ifndef OBJECT_H_
#define OBJECT_H_

#include <SDL2/SDL.h>

struct object {
	struct SDL_Texture *texture;
	struct SDL_Renderer *renderer;
	struct SDL_Rect dst;
	SDL_RendererFlip flip;
};

void object_init(struct object *object, struct SDL_Renderer *renderer,
		const struct SDL_Rect *rect, char *image[]);
void object_set_pos(struct object *object, const struct SDL_Rect *rect);
void object_render(struct object *object);
void object_toggle_flip(struct object *object, SDL_RendererFlip flip);
void object_cleanup(struct object *object);

#endif /* OBJECT_H_ */
