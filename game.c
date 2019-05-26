#include "game.h"
#include "alien_movement.h"
#include "explosion.h"

#define TICK_INTERVAL 16

void game_init(struct game *game, struct SDL_Renderer *renderer,
		struct SDL_Window *window) {
	unsigned i;

	ship_init(&game->ship, renderer);
	game->alien_popping_period = ALIEN_POPPING_PERIOD;
	game->nb_aliens = 0;
	game->renderer = renderer;
	game->window = window;
	game->alien_movement = 0;
	game->spawned_aliens = 0;
	input_init(&game->input, window);

	for (i = 0; i < GAME_MAX_EXPLOSIONS; i++)
		explosion_init(game->explosion + i, renderer);
	score_init(&game->score, renderer);

	message_init(&game->message, renderer, MESSAGE_ID_READY);
	game->current_praise = MESSAGE_ID_NOT_THAT_BAD;
	game->current_aliens_killed = 0;
	walls_init(&game->walls, renderer);
	stars_init(&game->stars, renderer);
	meteors_init(&game->meteors, renderer);
	game->next_time = SDL_GetTicks() + TICK_INTERVAL;
	game->music = Mix_LoadMUS(
			"res/music/Julius_Nox_-_Giulio_s_Page_-_Tortoise.mp3");
	Mix_PlayMusic(game->music, -1);
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
	if (game->spawned_aliens == GAME_MAX_ALIENS) {
		if (game->nb_aliens != 0)
			return;

		/* start spawning next alien wave */
		game->spawned_aliens = 0;
		if (game->current_aliens_killed == GAME_MAX_ALIENS) {
			message_init(&game->message, game->renderer,
					game->current_praise);
			if (game->current_praise != MESSAGE_ID_LIKE_A_GOD)
				game->current_praise++;
		} else {
			message_init(&game->message, game->renderer,
					message_random_taunt());
			game->current_praise = MESSAGE_ID_NOT_THAT_BAD;
		}
		game->current_aliens_killed = 0;
	}
	if (game->alien_popping_period > 0) {
		game->alien_popping_period--;
	} else {
		if (game->nb_aliens < GAME_MAX_ALIENS) {
			add_alien(game);
			game->spawned_aliens++;
		}
		game->alien_popping_period = ALIEN_POPPING_PERIOD;
		if (game->spawned_aliens == GAME_MAX_ALIENS) {
			game->alien_movement++;
			if (game->alien_movement == alien_movement_get_nb()) {
				game->alien_movement = 0;
				meteors_add(&game->meteors);
			}
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
					alien_get_bounding_box(alien),
					EXPLOSION_TYPE_ALIEN);
			explosion_start(find_dead_explosion(game),
					ship_get_bounding_box(&game->ship),
					EXPLOSION_TYPE_NONE);
			message_init(&game->message, game->renderer,
					MESSAGE_ID_LOOSER);
		}
		if (alien_shoot_collides(alien, ship_bb)) {
			ship_set_dead(&game->ship);
			alien_shoot_set_dead(alien);
			explosion_start(find_dead_explosion(game),
					ship_get_bounding_box(&game->ship),
					EXPLOSION_TYPE_ALIEN);
			message_init(&game->message, game->renderer,
					MESSAGE_ID_LOOSER);
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
			explosion_start(find_dead_explosion(game), alien_bb,
					EXPLOSION_TYPE_ALIEN);
			score_increase(&game->score, 1);
			game->current_aliens_killed++;
		}
	}
}

static void check_meteor_collisions(struct game *game)
{
	unsigned i;
	struct meteor *meteor;
	const struct SDL_Rect *ship_bb;

	for (i = 0; i < meteors_get_nb(&game->meteors); i++) {
		meteor = meteors_get(&game->meteors, i);
		ship_bb = ship_get_bounding_box(&game->ship);

		if (meteor_collides(meteor, ship_bb)) {
			meteor_hit(meteor);
			ship_set_dead(&game->ship);
			explosion_start(find_dead_explosion(game),
					ship_get_bounding_box(&game->ship),
					EXPLOSION_TYPE_ALIEN);
			message_init(&game->message, game->renderer,
					MESSAGE_ID_LOOSER);
		}
	}
}

static void check_meteor_shoot_collisions(struct game *game)
{
	unsigned i;
	struct meteor *meteor;

	for (i = 0; i < meteors_get_nb(&game->meteors); i++) {
		meteor = meteors_get(&game->meteors, i);
		if (meteor_is_dead(meteor))
			continue;
		if (ship_shoot_hits(&game->ship, &meteor->object.pos)) {
			meteor_hit(meteor);
			if (meteor_is_dead(meteor)) {
				explosion_start(find_dead_explosion(game),
						&meteor->object.pos,
						EXPLOSION_TYPE_METEOR);
				score_increase(&game->score, 3);
			}
		}
	}
}

static void check_collisions(struct game *game) {
	if (!ship_is_dead(&game->ship))
		check_alien_ship_collisions(game);
	check_alien_shoot_collisions(game);
	check_meteor_collisions(game);
	check_meteor_shoot_collisions(game);
}

static void update_explosions(struct game *game) {
	unsigned i;

	for (i = 0; i < GAME_MAX_EXPLOSIONS; i++)
		explosion_update(game->explosion + i);
}

static Uint32 time_left(Uint32 next_time)
{
	Uint32 now;

	now = SDL_GetTicks();

	return next_time <= now ? 0 : next_time - now;
}

static void game_reset(struct game *game)
{
	struct SDL_Renderer *renderer;
	struct SDL_Window *window;

	renderer = game->renderer;
	window = game->window;
	game_cleanup(game);
	game_init(game, renderer, window);
}

void game_update(struct game *game) {
	enum message_id id;
	Uint32 delay;
	bool reset;

	reset = false;
	stars_update(&game->stars);
	walls_update(&game->walls);
	input_update(&game->input);
	if (game->input.pause)
		return;
	ship_update(&game->ship, &game->input);
	if (game->message.id > MESSAGE_ID_GO || message_is_dead(&game->message))
		meteors_update(&game->meteors);
	if (message_is_dead(&game->message)) {
		if (!ship_is_dead(&game->ship))
			spawn_aliens(game);
		update_aliens(game);
		check_collisions(game);
	} else {
		id = game->message.id;
		message_update(&game->message);
		/* TODO replace with some way to restart the game */
		if (message_is_dead(&game->message) && id == MESSAGE_ID_LOOSER) {
			reset = true;
			printf("Final score %u\n", game->score.value);
		}
	}
	update_explosions(game);
	score_update(&game->score);

	delay = time_left(game->next_time);
	SDL_Delay(delay);
	game->next_time += TICK_INTERVAL;
	if (reset)
		game_reset(game);
}

void game_cleanup(struct game *game) {
	unsigned i;

	Mix_PauseMusic();
	Mix_FreeMusic(game->music);
	meteors_cleanup(&game->meteors);
	stars_cleanup(&game->stars);
	walls_cleanup(&game->walls);
	message_cleanup(&game->message);
	for (i = 0; i < game->nb_aliens; i++)
		alien_cleanup(game->alien + i);
	input_cleanup(&game->input);
	ship_cleanup(&game->ship);
}
