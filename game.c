#include "game.h"
#include "alien_movement.h"
#include "explosion.h"

void game_init(struct game *game, struct SDL_Renderer *renderer) {
	unsigned i;

	ship_init(&game->ship, renderer);
	game->alien_popping_period = ALIEN_POPPING_PERIOD;
	game->nb_aliens = 0;
	game->renderer = renderer;
	game->alien_movement = 0;
	game->spawned_aliens = 0;
	input_init(&game->input);

	for (i = 0; i < GAME_MAX_EXPLOSIONS; i++)
		explosion_init(game->explosion + i, renderer);
	score_init(&game->score, renderer);
}

static void add_alien(struct game *game) {
	alien_init(game->alien + game->nb_aliens, game->renderer,
			game->alien_movement, game->spawned_aliens);
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
		if (game->nb_aliens < GAME_MAX_ALIENS) {
			add_alien(game);
			game->spawned_aliens++;
		}
		game->alien_popping_period = ALIEN_POPPING_PERIOD;
		if (game->spawned_aliens == GAME_MAX_ALIENS) {
			game->alien_popping_period *= 10;
			game->alien_movement++;
			if (game->alien_movement == alien_movement_get_nb())
				game->alien_movement = 0;
			game->spawned_aliens = 0;
		}
	}
}

static struct explosion *find_dead_explosion(struct game *game) {
	unsigned i;
	struct explosion *explosion;

	for (i = 0; i < GAME_MAX_EXPLOSIONS; i++) {
		explosion = game->explosion + i;
		if (explosion_is_dead(explosion))
			return explosion;
	}

	return NULL;
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
			alien_set_dead(alien);
			explosion_start(find_dead_explosion(game),
					alien_get_bounding_box(alien));
			explosion_start(find_dead_explosion(game),
					ship_get_bounding_box(&game->ship));
			/* TODO add game over screen */
		}
		if (alien_shoot_collides(alien, ship_bb)) {
			ship_set_dead(&game->ship);
			alien_shoot_set_dead(alien);
			explosion_start(find_dead_explosion(game),
					ship_get_bounding_box(&game->ship));
			/* TODO add game over screen */
		}
	}
}

static void check_alien_shoot_collisions(struct game *game) {
	unsigned i;
	struct alien *alien;
	const struct SDL_Rect *alien_bb;

	for (i = 0; i < game->nb_aliens; i++) {
		alien = game->alien + i;
		alien_bb = alien_get_bounding_box(alien);
		if (ship_shoot_hits(&game->ship, alien_bb)) {
			alien_set_dead(alien);
			explosion_start(find_dead_explosion(game),
					alien_get_bounding_box(alien));
			score_increase(&game->score);
		}
	}
}

static void check_collisions(struct game *game) {
	if (!ship_is_dead(&game->ship))
		check_alien_ship_collisions(game);
	check_alien_shoot_collisions(game);
}

static void update_explosions(struct game *game) {
	unsigned i;

	for (i = 0; i < GAME_MAX_EXPLOSIONS; i++)
		explosion_update(game->explosion + i);
}

void game_update(struct game *game) {
	input_update(&game->input);
	ship_update(&game->ship, &game->input);
	if (!ship_is_dead(&game->ship))
		spawn_aliens(game);
	update_aliens(game);
	check_collisions(game);
	update_explosions(game);
	score_update(&game->score);
}

void game_cleanup(struct game *game) {
	unsigned i;

	for (i = 0; i < game->nb_aliens; i++)
		alien_cleanup(game->alien + i);
	input_cleanup(&game->input);
	ship_cleanup(&game->ship);
}
