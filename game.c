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
static void spawn_aliens(struct game *game) {
	if (game->alien_popping_period > 0) {
		game->alien_popping_period--;
	} else {
		if (game->nb_aliens < GAME_MAX_ALIENS)
			add_alien(game);
		game->alien_popping_period = ALIEN_POPPING_PERIOD;
		if (game->nb_aliens == GAME_MAX_ALIENS)
			game->alien_popping_period *= 10;
	}
}

static void check_alien_ship_collisions(struct game *game) {
	unsigned i;
	struct alien *alien;
	const struct SDL_Rect *ship_bb;

	ship_bb = ship_get_bounding_box(&game->ship);
	for (i = 0; i < game->nb_aliens; i++) {
		alien = game->alien + i;
		if (alien_collides(alien, ship_bb)) {
			ship_set_dead(&game->ship);
		}
	}
}

static void check_collisions(struct game *game) {
	if (!ship_is_dead(&game->ship))
		check_alien_ship_collisions(game);
}

void game_update(struct game *game) {
	input_update(&game->input);
	ship_update(&game->ship, &game->input);
	if (!ship_is_dead(&game->ship))
		spawn_aliens(game);
	update_aliens(game);
	check_collisions(game);
}

void game_cleanup(struct game *game) {
	unsigned i;

	for (i = 0; i < game->nb_aliens; i++)
		alien_cleanup(game->alien + i);
	input_cleanup(&game->input);
	ship_cleanup(&game->ship);
}
