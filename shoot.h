#ifndef SHOOT_H_
#define SHOOT_H_
#include <stdbool.h>

#include <SDL2/SDL_mixer.h>

#include "object.h"

struct shoot {
	struct object object;
	bool dead;
	int increment;
	Mix_Chunk *sound;
};

void shoot_init(struct shoot *shoot, struct SDL_Renderer *renderer,
		int increment);
void shoot_shoot(struct shoot *shoot, const struct SDL_Rect *rect);
void shoot_update(struct shoot *shoot);
bool shoot_is_dead(const struct shoot *shoot);
void shoot_set_dead(struct shoot *shoot, bool dead);
bool shoot_collides(const struct shoot *shoot, const struct SDL_Rect *rect);
void shoot_cleanup(struct shoot *shoot);

#endif /* SHOOT_H_ */
