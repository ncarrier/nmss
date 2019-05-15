#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ship.h"
#include "screen.h"
#include "object.h"

#include "res/ship.xpm"

#define INTERSHOOT_DELAY 20

static const struct SDL_Rect init_pos = { .x = 10, .y = 28, .w = 8, .h = 8 };

static void update_ship_bounding_box(struct ship *ship) {
	ship->bounding_box = (struct SDL_Rect) {
		.x = ship->object.dst.x,
		.y = ship->object.dst.y + 2,
		.w = ship->object.dst.w,
		.h = ship->object.dst.h - 4,
	};
}

void ship_init(struct ship *ship, struct SDL_Renderer *renderer) {
	unsigned i;

	object_init(&ship->object, renderer, &init_pos, ship_xpm);
	ship->intershoot_delay = 0;
	for (i = 0; i < SHIP_MAX_SHOOTS; i++)
		shoot_init(ship->shoot + i, ship->object.renderer, 3);
	ship->nb_shoots = 0;
	update_ship_bounding_box(ship);
	ship->dead = false;
	ship->speed = 2;
}

static struct shoot *find_dead_shoot(struct ship *ship) {
	unsigned i;
	struct shoot *shoot;

	for (i = 0; i < SHIP_MAX_SHOOTS; i++) {
		shoot = ship->shoot + i;
		if (shoot_is_dead(shoot))
			return shoot;
	}

	return NULL;
}

static void do_shoot(struct ship *ship) {
	struct shoot *shoot;

	if (ship->intershoot_delay == 0 && ship->nb_shoots < SHIP_MAX_SHOOTS) {
		shoot = find_dead_shoot(ship);
		ship->intershoot_delay = INTERSHOOT_DELAY;
		shoot_shoot(shoot, &ship->object.dst);
		ship->nb_shoots++;
	}
}

static void remove_shoot(struct ship *ship, unsigned i) {
	ship->nb_shoots--;
}

static void update_shoots(struct ship *ship) {
	unsigned i;
	struct shoot *shoot;

	for (i = 0; i < SHIP_MAX_SHOOTS; i++) {
		shoot = ship->shoot + i;
		if (shoot_is_dead(shoot))
			continue;
		shoot_update(shoot);
		if (shoot_is_dead(shoot))
			remove_shoot(ship, i);
	}
}

static void move_ship(struct ship *ship, struct input *input) {
	if (input->down_down) {
		ship->object.dst.y += ship->speed;
		if (ship->object.dst.y > SCREEN_HEIGHT - 6)
			ship->object.dst.y -= ship->speed;
	}
	if (input->up_down) {
		ship->object.dst.y -= ship->speed;
		if (ship->object.dst.y < - 2)
			ship->object.dst.y += ship->speed;
	}
	if (input->left_down) {
		ship->object.dst.x -= ship->speed;
		if (ship->object.dst.x < 0)
			ship->object.dst.x += ship->speed;
	}
	if (input->right_down) {
		ship->object.dst.x += ship->speed;
		if (ship->object.dst.x > SCREEN_WIDTH - 8)
			ship->object.dst.x -= ship->speed;
	}
	update_ship_bounding_box(ship);
}

void ship_update(struct ship *ship, struct input *input) {
	if (!ship_is_dead(ship)) {
		move_ship(ship, input);
		object_render(&ship->object);
		if (input->a_down)
			do_shoot(ship);
		if (ship->intershoot_delay > 0)
			ship->intershoot_delay--;
	}

	update_shoots(ship);
}

const struct SDL_Rect *ship_get_bounding_box(const struct ship *ship) {
	return &ship->bounding_box;
}

bool ship_is_dead(const struct ship *ship) {
	return ship->dead;
}

void ship_set_dead(struct ship *ship) {
	ship->dead = true;
}

bool ship_shoot_hits(struct ship *ship, const struct SDL_Rect *rect) {
	unsigned i;
	struct shoot *shoot;
	bool hits;

	for (i = 0; i < ship->nb_shoots; i++) {
		shoot = ship->shoot + i;
		hits = shoot_collides(shoot, rect);
		if (hits) {
			shoot_set_dead(shoot, true);
			remove_shoot(ship, i);
			return hits;
		}
	}

	return false;
}

void ship_cleanup(struct ship *ship) {
	unsigned i;

	for (i = 0; i < SHIP_MAX_SHOOTS; i++)
		shoot_cleanup(ship->shoot + i);

	object_cleanup(&ship->object);
}
