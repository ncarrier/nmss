#ifndef SHIP_H_
#define SHIP_H_
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "input.h"
#include "object.h"
#include "shoot.h"

#define SHIP_MAX_SHOOTS 3

struct ship {
	struct object object;
	unsigned intershoot_delay;
	struct shoot shoot[SHIP_MAX_SHOOTS];
	unsigned nb_shoots;
	struct SDL_Rect bounding_box;
	bool dead;
};

void ship_init(struct ship *ship, struct SDL_Renderer *renderer);
void ship_update(struct ship *ship, struct input *input);
const struct SDL_Rect *ship_get_bounding_box(const struct ship *ship);
bool ship_is_dead(const struct ship *ship);
void ship_set_dead(struct ship *ship);
bool ship_shoot_hits(struct ship *, const struct SDL_Rect *rect);
void ship_cleanup(struct ship *ship);

#endif /* SHIP_H_ */
