#include "shoot.h"
#include "screen.h"

#define SHOOT_IMAGE "res/shoot.png"

void shoot_init(struct shoot *shoot, struct SDL_Renderer *renderer,
		const struct SDL_Rect *ship_pos)
{
	struct SDL_Rect shoot_init_pos;

	shoot_init_pos = *ship_pos;
	shoot_init_pos.x += 8;
	object_init(&shoot->object, renderer, &shoot_init_pos, SHOOT_IMAGE);
	shoot->dead = false;
}

void shoot_update(struct shoot *shoot) {
	object_render(&shoot->object);
	shoot->object.dst.x += 2;
	if (shoot->object.dst.x > SCREEN_WIDTH)
		shoot_set_dead(shoot);
}

bool shoot_is_dead(const struct shoot *shoot)
{
	return shoot->dead;
}

void shoot_set_dead(struct shoot *shoot)
{
	shoot->dead = true;
}

bool shoot_collides(const struct shoot *shoot, const struct SDL_Rect *rect) {
	return SDL_HasIntersection(&shoot->object.dst, rect);
}

void shoot_cleanup(struct shoot *shoot)
{
	object_cleanup(&shoot->object);
}
