#ifndef SCORE_H_
#define SCORE_H_

#include <SDL2/SDL.h>

struct score {
	struct SDL_Renderer *renderer;
	struct SDL_Texture *texture;
	unsigned value;
};

void score_init(struct score *score, struct SDL_Renderer *renderer);
void score_update(struct score *score);
void score_increase(struct score *score, unsigned increment);
void score_cleanup(struct score *score);

#endif /* SCORE_H_ */
