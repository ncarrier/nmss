#include "game.h"

void game_init(struct game *game, struct SDL_Renderer *renderer) {
	ship_init(&game->ship, renderer);
	game->alien_popping_period = ALIEN_POPPING_PERIOD;
	game->nb_aliens = 0;
	game->renderer = renderer;
	input_init(&game->input);
}

static void add_alien(struct game *game) {
	alien_init(game->alien + game->nb_aliens, game->renderer);
	game->nb_aliens++;
}

static void remove_alien(struct game *game, unsigned i) {
	unsigned j;

	alien_cleanup(game->alien + i);
	for (j = i; j < GAME_MAX_ALIENS- 1; j++)
		game->alien[j] = game->alien[j + 1];
	game->nb_aliens--;
}

static void update_aliens(struct game *game) {
	unsigned i;
	struct alien *alien;

	for (i = 0; i < game->nb_aliens; ) {
		alien = game->alien + i;
		alien_update(alien);
		if (alien_is_dead(alien))
			remove_alien(game, i);
		else
			i++;
	}
}

void game_update(struct game *game) {
	input_update(&game->input);
	ship_update(&game->ship, &game->input);
	if (game->alien_popping_period > 0) {
		game->alien_popping_period--;
	} else {
		if (game->nb_aliens < GAME_MAX_ALIENS)
			add_alien(game);
		game->alien_popping_period = ALIEN_POPPING_PERIOD;
		if (game->nb_aliens == GAME_MAX_ALIENS)
			game->alien_popping_period *= 10;
	}

	update_aliens(game);
}

void game_cleanup(struct game *game) {
	unsigned i;

	for (i = 0; i < game->nb_aliens; i++)
		alien_cleanup(game->alien + i);
	input_cleanup(&game->input);
	ship_cleanup(&game->ship);
}
