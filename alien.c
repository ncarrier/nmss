#include "alien.h"
#include "screen.h"

#define ALIEN_IMAGE "res/alien.png"
#define ALIEN_FLIP_PERIOD 20

static const struct SDL_Rect alien_init_pos = {
		.x = SCREEN_WIDTH,
		.y = SCREEN_HEIGHT / 2 - SCREEN_SPRITE_HEIGHT / 2,
		.w = SCREEN_SPRITE_WIDTH,
		.h = SCREEN_SPRITE_HEIGHT
};

void alien_init(struct alien *alien, struct SDL_Renderer *renderer,
		int movement, unsigned alien_index) {
	object_init(&alien->object, renderer, &alien_init_pos, ALIEN_IMAGE);
	alien->dead = false;
	alien->flip_counter = ALIEN_FLIP_PERIOD;
	alien_movement_init(&alien->movement, movement, alien_index);
}

void alien_update(struct alien *alien) {
	object_render(&alien->object);
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
	alien->flip_counter--;
	if (alien->flip_counter == 0) {
		object_toggle_flip(&alien->object, SDL_FLIP_VERTICAL);
		alien->flip_counter = ALIEN_FLIP_PERIOD;
	}
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

const struct SDL_Rect *alien_get_bounding_box(const struct alien *alien) {
	return &alien->object.dst;
}

void alien_cleanup(struct alien *alien) {
	object_cleanup(&alien->object);
}
