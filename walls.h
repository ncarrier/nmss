#ifndef WALLS_H_
#define WALLS_H_

#include <SDL2/SDL.h>

#define WALLS_POINTS 10

struct walls {
	struct SDL_Point points[WALLS_POINTS];
	struct SDL_Renderer *renderer;
};

void walls_init(struct walls *walls, struct SDL_Renderer *renderer);
void walls_update(struct walls *walls);
void walls_cleanup(struct walls *walls);

#endif /* WALLS_H_ */
