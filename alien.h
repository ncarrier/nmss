#ifndef ALIEN_H_
#define ALIEN_H_
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "object.h"

struct alien {
	struct object object;
	bool dead;
	unsigned flip_counter;
};

void alien_init(struct alien *alien, struct SDL_Renderer *renderer);
void alien_update(struct alien *alien);
bool alien_is_dead(const struct alien *alien);
void alien_set_dead(struct alien *alien);
bool alien_collides(const struct alien *alien, const struct SDL_Rect *rect);
void alien_cleanup(struct alien *alien);

#endif /* ALIEN_H_ */
