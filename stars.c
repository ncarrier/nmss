#include "stars.h"
#include "utils.h"
#include "screen.h"

static struct star get_star(void)
{
	struct star star = {0};

	star.luminance = random_in_range(20, 128);
	star.point.x = SCREEN_WIDTH;
	star.point.y = random_in_range(0, 2 * SCREEN_HEIGHT / 3);
	star.speed = random_in_range(2, 5);

	return star;
}

void stars_init(struct stars *stars, struct SDL_Renderer *renderer)
{
	unsigned i;

	stars->renderer = renderer;
	for (i = 0; i < STARS_NB; i++) {
		stars->stars[i] = get_star();
		stars->stars[i].point.x = random_in_range(0, SCREEN_WIDTH);
	}
}

void stars_update(struct stars *stars)
{
	unsigned i;
	struct star *star;

	for (i = 0; i < STARS_NB; i++) {
		star = stars->stars + i;
		SDL_SetRenderDrawColor(stars->renderer, star->luminance,
				star->luminance, star->luminance,
				SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(stars->renderer, star->point.x,
				star->point.y, star->point.x + star->speed * 2,
				star->point.y);
		star->point.x -= star->speed;
		if (star->point.x < 0)
			*star = get_star();
	}
}

void stars_cleanup(struct stars *stars)
{

}
