#ifndef SHOOT_H_
#define SHOOT_H_
#include <stdbool.h>

#include "object.h"

struct shoot {
	struct object object;
	bool dead;
};

void shoot_init(struct shoot *shoot, struct SDL_Renderer *renderer,
		const struct SDL_Rect *ship_pos);
void shoot_update(struct shoot *shoot);
bool shoot_is_dead(const struct shoot *shoot);
void shoot_set_dead(struct shoot *shoot);
void shoot_cleanup(struct shoot *shoot);

#endif /* SHOOT_H_ */
