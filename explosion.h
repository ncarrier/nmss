#ifndef EXPLOSION_H_
#define EXPLOSION_H_
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "object.h"

struct explosion {
	struct object explode_1;
	struct object explode_2;
	struct object explode_3;
	unsigned phase;
	Mix_Chunk *sound;
};

void explosion_init(struct explosion *explosion, struct SDL_Renderer *renderer);
void explosion_update(struct explosion *explosion);
void explosion_start(struct explosion *explosion, const struct SDL_Rect *pos);
bool explosion_is_dead(const struct explosion *explosion);
void explosion_cleanup(struct explosion *explosion);

#endif /* EXPLOSION_H_ */
