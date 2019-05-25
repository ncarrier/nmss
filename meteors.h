#ifndef METEORS_H_
#define METEORS_H_
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "object.h"
#include "utils.h"

#define METEORS_NB 20

struct meteor {
	struct double_point speed;
	struct double_point position;
	struct object object;
	bool dead;
};
struct meteors {
	struct meteor meteors[METEORS_NB];
	unsigned count;
};

void meteors_init(struct meteors *meteors, struct SDL_Renderer *renderer);
void meteors_update(struct meteors *meteors);
unsigned meteors_get_nb(struct meteors *meteors);
void meteors_add(struct meteors *meteors);
bool meteor_collides(struct meteor *meteor, const struct SDL_Rect *bb);
struct meteor *meteors_get(struct meteors *meteors, unsigned i);
bool meteor_is_dead(struct meteor *meteor);
void meteor_set_dead(struct meteor *meteor);
void meteors_cleanup(struct meteors *meteors);

#endif /* METEORS_H_ */
