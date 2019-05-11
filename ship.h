#ifndef SHIP_H_
#define SHIP_H_
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
};

void ship_init(struct ship *ship, struct SDL_Renderer *renderer);
void ship_update(struct ship *ship, struct input *input);
void ship_cleanup(struct ship *ship);

#endif /* SHIP_H_ */
