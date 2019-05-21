#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>

#include "ship.h"
#include "alien.h"
#include "input.h"
#include "explosion.h"
#include "score.h"
#include "message.h"

#define GAME_MAX_ALIENS 12
#define ALIEN_POPPING_PERIOD 30
#define GAME_MAX_EXPLOSIONS 13

struct game {
	struct ship ship;
	unsigned alien_popping_period;
	bool all_aliens_dead;
	struct alien alien[GAME_MAX_ALIENS];
	unsigned nb_aliens;
	unsigned spawned_aliens;
	struct input input;
	struct SDL_Renderer *renderer;
	unsigned alien_movement;
	struct explosion explosion[GAME_MAX_EXPLOSIONS];
	struct score score;
	struct message message;
	/* in what follows, current stands for "in the current wave" */
	enum message_id current_praise;
	unsigned current_aliens_killed;
};

void game_init(struct game *game, struct SDL_Renderer *renderer);
void game_update(struct game *game);
void game_cleanup(struct game *game);

#endif /* GAME_H_ */
