#ifndef ALIEN_H_
#define ALIEN_H_
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "object.h"
#include "alien_movement.h"

struct alien {
	struct object object;
	bool dead;
	unsigned flip_counter;
	struct alien_movement movement;
};

void alien_init(struct alien *alien, struct SDL_Renderer *renderer,
		int movement, unsigned alien_index);
void alien_update(struct alien *alien);
bool alien_is_dead(const struct alien *alien);
void alien_set_dead(struct alien *alien);
bool alien_collides(const struct alien *alien, const struct SDL_Rect *rect);
const struct SDL_Rect *alien_get_bounding_box(const struct alien *alien);
void alien_cleanup(struct alien *alien);

#endif /* ALIEN_H_ */
