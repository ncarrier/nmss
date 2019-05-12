#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>

#include "ship.h"
#include "alien.h"
#include "input.h"

#define GAME_MAX_ALIENS 6
#define ALIEN_POPPING_PERIOD 30

struct game {
	struct ship ship;
	unsigned alien_popping_period;
	struct alien alien[GAME_MAX_ALIENS];
	unsigned nb_aliens;
	unsigned spawned_aliens;
	struct input input;
	struct SDL_Renderer *renderer;
	unsigned alien_movement;
};

void game_init(struct game *game, struct SDL_Renderer *renderer);
void game_update(struct game *game);
void game_cleanup(struct game *game);

#endif /* GAME_H_ */
