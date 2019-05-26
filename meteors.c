#include "meteors.h"
#include "utils.h"
#include "screen.h"

#include "res/meteor.xpm"
#include "res/meteor_fragment.xpm"

#define METEOR_MAX_HITS 3

static struct SDL_Point get_point(int x)
{
	struct SDL_Point result;

	result = (struct SDL_Point) {
		.x = x,
		.y = random_in_range(0, SCREEN_HEIGHT),
	};

	return result;
}

static void meteor_init(struct meteor *meteor)
{
	struct SDL_Rect init_pos = {
		.x = 0, /* placeholder */
		.y = 0, /* placeholder */
		.w = SCREEN_SPRITE_WIDTH,
		.h = SCREEN_SPRITE_HEIGHT,
	};
	struct SDL_Point origin;
	struct SDL_Point destination;
	double norm;

	origin = get_point(SCREEN_WIDTH);
	destination = get_point(0);
	/* make the meteor cross the center stripe */
	if ((origin.y < SCREEN_HEIGHT / 4 ||
				origin.y > 3 * SCREEN_HEIGHT / 4) &&
			abs(origin.y - destination.y) < SCREEN_HEIGHT / 2) {
		if (origin.y < SCREEN_HEIGHT / 4)
			origin.y += SCREEN_HEIGHT / 2;
		else
			origin.y -= SCREEN_HEIGHT / 2;
	}

	meteor->position = (struct double_point) {
		.x = origin.x,
		.y = origin.y,
	};
	meteor->speed = (struct double_point) {
		.x = destination.x - origin.x,
		.y = destination.y - origin.y,
	};

	norm = sqrt(meteor->speed.x * meteor->speed.x +
			meteor->speed.y * meteor->speed.y);
	meteor->speed.x *= 2. / norm;
	meteor->speed.y *= 2. / norm;
	init_pos.x = origin.x;
	init_pos.y = origin.y;
	object_set_pos(&meteor->object, &init_pos);
	meteor->hits = 0;
}

void meteors_init(struct meteors *meteors, struct SDL_Renderer *renderer)
{
	unsigned i;
	struct meteor *meteor;
	static const struct SDL_Rect init_pos = {
		.x = 0, /* placeholder */
		.y = 0, /* placeholder */
		.w = SCREEN_SPRITE_WIDTH,
		.h = SCREEN_SPRITE_HEIGHT,
	};
	unsigned j;
	struct meteor_fragment *fragment;

	for (i = 0; i < METEORS_NB; i++) {
		meteor = meteors->meteors + i;
		object_init(&meteor->object, renderer, &init_pos, meteor_xpm);
		meteor_init(meteor);
		for (j = 0; j < 4; j++) {
			fragment = meteor->fragment + j;
			object_init(&fragment->object, renderer, &init_pos,
					meteor_fragment_xpm);
		}
	}
	meteors->count = 0;
}

void meteors_update(struct meteors *meteors)
{
	unsigned i;
	struct meteor *meteor;
	static const struct {
		unsigned x;
		unsigned y;
	} velocity[4] = {
			[0] = { .x = 1, .y = 1},
			[1] = { .x = -1, .y = 1},
			[2] = { .x = 1, .y = -1},
			[3] = { .x = -1, .y = -1},
	};
	unsigned j;
	struct meteor_fragment *fragment;

	for (i = 0; i < meteors->count; i++) {
		meteor = meteors->meteors + i;
		if (!meteor_is_dead(meteor)) {
			object_render(&meteor->object);
		} else {
			for (j = 0; j < 4; j++) {
				fragment = meteor->fragment + j;
				object_render(&fragment->object);
				fragment->object.pos.x += velocity[j].x;
				fragment->object.pos.y += velocity[j].y;
			}
		}
		meteor->position.x += meteor->speed.x;
		meteor->position.y += meteor->speed.y;
		meteor->object.pos.x = meteor->position.x;
		meteor->object.pos.y = meteor->position.y;
		if (meteor->position.x < -SCREEN_WIDTH / 5)
			meteor_init(meteor);
	}
}

unsigned meteors_get_nb(struct meteors *meteors)
{
	return meteors->count;
}

struct meteor *meteors_get(struct meteors *meteors, unsigned i)
{
	return meteors->meteors + i;
}

bool meteor_collides(struct meteor *meteor, const struct SDL_Rect *bb)
{
	unsigned j;
	struct meteor_fragment *fragment;

	if (meteor_is_dead(meteor)) {
		for (j = 0; j < 4; j++) {
			fragment = meteor->fragment + j;
			if (SDL_HasIntersection(&fragment->object.pos, bb))
				return true;
		}
	} else {
		return SDL_HasIntersection(&meteor->object.pos, bb);
	}

	return false;
}

void meteors_add(struct meteors *meteors)
{
	meteors->count++;
}

bool meteor_is_dead(struct meteor *meteor)
{
	return meteor->hits == METEOR_MAX_HITS;
}

void meteor_hit(struct meteor *meteor)
{
	unsigned i;
	struct meteor_fragment *fragment;

	meteor->hits++;
	if (meteor_is_dead(meteor)) {
		for (i = 0; i < 4; i++) {
			fragment = meteor->fragment + i;
			object_set_pos(&fragment->object, &meteor->object.pos);
		}
	}
}

void meteors_cleanup(struct meteors *meteors)
{
	unsigned i;

	for (i = 0; i < METEORS_NB; i++)
		object_cleanup(&meteors->meteors[i].object);
}
