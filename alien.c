#include "alien.h"
#include "screen.h"

#include "res/alien.xpm"

#define ALIEN_FLIP_PERIOD 20

static const struct SDL_Rect alien_init_pos = {
		.x = SCREEN_WIDTH,
		.y = SCREEN_HEIGHT / 2 - SCREEN_SPRITE_HEIGHT / 2,
		.w = SCREEN_SPRITE_WIDTH,
		.h = SCREEN_SPRITE_HEIGHT
};

void alien_init(struct alien *alien, struct SDL_Renderer *renderer,
		int movement, unsigned alien_index) {
	object_init(&alien->object, renderer, &alien_init_pos, alien_xpm);
	alien->dead = false;
	alien->flip_counter = ALIEN_FLIP_PERIOD;
	alien_movement_init(&alien->movement, movement, alien_index);
	shoot_init(&alien->shoot, renderer, -2);
}

void alien_update_pos(struct alien *alien) {
	alien->object.dst.x = alien_movement_get_x(
			&alien->movement,
			alien->object.dst.x,
			alien->object.dst.y);
	alien->object.dst.y = alien_movement_get_y(
			&alien->movement,
			alien->object.dst.x,
			alien->object.dst.y);
	if (alien->object.dst.x < -SCREEN_SPRITE_WIDTH)
		alien_set_dead(alien);
}

void alien_update_flip(struct alien *alien) {
	alien->flip_counter--;
	if (alien->flip_counter == 0) {
		object_toggle_flip(&alien->object, SDL_FLIP_VERTICAL);
		alien->flip_counter = ALIEN_FLIP_PERIOD;
	}
}

static void alien_update_shoot(struct alien *alien) {
	if (shoot_is_dead(&alien->shoot)) {
		if (random() % 40 == 0)
			shoot_shoot(&alien->shoot, &alien->object.dst);
	} else {
		shoot_update(&alien->shoot);
	}
}

void alien_update(struct alien *alien) {
	object_render(&alien->object);
	alien_update_pos(alien);
	alien_update_flip(alien);
	alien_update_shoot(alien);
	alien_movement_update(&alien->movement);
}

bool alien_is_dead(const struct alien *alien) {
	return alien->dead;
}

void alien_set_dead(struct alien *alien)
{
	alien->dead = true;
}

bool alien_collides(const struct alien *alien, const struct SDL_Rect *rect) {
	return SDL_HasIntersection(&alien->object.dst, rect);
}

bool alien_shoot_collides(const struct alien *alien,
		const struct SDL_Rect *rect) {
	return !shoot_is_dead(&alien->shoot) &&
			shoot_collides(&alien->shoot, rect);
}

void alien_shoot_set_dead(struct alien *alien)
{
	shoot_set_dead(&alien->shoot, true);
}

const struct SDL_Rect *alien_get_bounding_box(const struct alien *alien) {
	return &alien->object.dst;
}

void alien_cleanup(struct alien *alien) {
	object_cleanup(&alien->object);
}
