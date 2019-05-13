#include "shoot.h"
#include "screen.h"

#define SHOOT_IMAGE "res/shoot.png"

void shoot_init(struct shoot *shoot, struct SDL_Renderer *renderer,
		bool forward) {
	object_init(&shoot->object, renderer, NULL, SHOOT_IMAGE);
	shoot_set_dead(shoot, true);
	shoot->increment = forward ? 2 : -2;
}

void shoot_shoot(struct shoot *shoot, const struct SDL_Rect *rect) {
	shoot_set_dead(shoot, false);
	object_set_pos(&shoot->object, rect);
}

void shoot_update(struct shoot *shoot) {
	object_render(&shoot->object);
	shoot->object.dst.x += shoot->increment;
	if (shoot->object.dst.x > SCREEN_WIDTH)
		shoot_set_dead(shoot, true);
}

bool shoot_is_dead(const struct shoot *shoot)
{
	return shoot->dead;
}

void shoot_set_dead(struct shoot *shoot, bool dead)
{
	shoot->dead = dead;
}

bool shoot_collides(const struct shoot *shoot, const struct SDL_Rect *rect) {
	return SDL_HasIntersection(&shoot->object.dst, rect);
}

void shoot_cleanup(struct shoot *shoot)
{
	object_cleanup(&shoot->object);
}
