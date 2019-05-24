#ifndef STARS_H_
#define STARS_H_

#include <SDL2/SDL.h>

#define STARS_NB 20

struct star {
	struct SDL_Point point;
	Uint8 luminance;
	unsigned speed;
};
struct stars {
	struct star stars[STARS_NB];
	struct SDL_Renderer *renderer;
};

void stars_init(struct stars *stars, struct SDL_Renderer *renderer);
void stars_update(struct stars *stars);
void stars_cleanup(struct stars *stars);

#endif /* STARS_H_ */
