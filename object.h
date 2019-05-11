#ifndef OBJECT_H_
#define OBJECT_H_

#include <SDL2/SDL.h>

struct object {
	struct SDL_Texture *texture;
	struct SDL_Renderer *renderer;
	struct SDL_Rect dst;
};

void object_init(struct object *object, struct SDL_Renderer *renderer,
		const struct SDL_Rect *rect, const char *image);
void object_render(struct object *object);
void object_cleanup(struct object *object);

#endif /* OBJECT_H_ */
