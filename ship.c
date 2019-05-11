#include <stdlib.h>
#include <error.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ship.h"
#include "screen.h"
#include "object.h"

#define SHIP_IMAGE "res/ship.png"
#define INTERSHOOT_DELAY 20

static const struct SDL_Rect init_pos = { .x = 10, .y = 28, .w = 8, .h = 8 };

void ship_init(struct ship *ship, struct SDL_Renderer *renderer) {
	object_init(&ship->object, renderer, &init_pos, SHIP_IMAGE);
	ship->intershoot_delay = 0;
	ship->nb_shoots = 0;
}

static void do_shoot(struct ship *ship) {
	if (ship->intershoot_delay == 0 && ship->nb_shoots < SHIP_MAX_SHOOTS) {
		ship->intershoot_delay = INTERSHOOT_DELAY;
		shoot_init(ship->shoot + ship->nb_shoots, ship->object.renderer,
				&ship->object.dst);
		ship->nb_shoots++;
	}
}

static void remove_shoot(struct ship *ship, unsigned i) {
	unsigned j;

	shoot_cleanup(ship->shoot + i);
	for (j = i; j < SHIP_MAX_SHOOTS - 1; j++)
		ship->shoot[j] = ship->shoot[j + 1];
	ship->nb_shoots--;
}

static void update_shoots(struct ship *ship) {
	unsigned i;
	struct shoot *shoot;

	for (i = 0; i < ship->nb_shoots; ) {
		shoot = ship->shoot + i;
		shoot_update(shoot);
		if (shoot_is_dead(shoot))
			remove_shoot(ship, i);
		else
			i++;
	}
}

static void move_ship(struct ship *ship, struct input *input) {
	if (input->down_down) {
		ship->object.dst.y++;
		if (ship->object.dst.y > SCREEN_HEIGHT - 6)
			ship->object.dst.y--;
	}
	if (input->up_down) {
		ship->object.dst.y--;
		if (ship->object.dst.y < - 2)
			ship->object.dst.y++;
	}
	if (input->left_down) {
		ship->object.dst.x--;
		if (ship->object.dst.x < 0)
			ship->object.dst.x++;
	}
	if (input->right_down) {
		ship->object.dst.x++;
		if (ship->object.dst.x > SCREEN_WIDTH - 8)
			ship->object.dst.x--;
	}
}

void ship_update(struct ship *ship, struct input *input) {
	move_ship(ship, input);
	object_render(&ship->object);

	if (input->a_down)
		do_shoot(ship);
	if (ship->intershoot_delay > 0)
		ship->intershoot_delay--;
	update_shoots(ship);
}

void ship_cleanup(struct ship *ship) {
	object_cleanup(&ship->object);
}
