#include "walls.h"
#include "utils.h"
#include "screen.h"

#define STEP (SCREEN_WIDTH / (WALLS_POINTS - 3))
#define BASE_LUMINANCE 20

static struct SDL_Point get_point(int i)
{
	return (struct SDL_Point) {
		.x = STEP * (i - 1),
		.y = random_in_range(2 * SCREEN_HEIGHT / 3,
				SCREEN_HEIGHT),
	};
}

void walls_init(struct walls *walls, struct SDL_Renderer *renderer)
{
	unsigned i;

	walls->renderer = renderer;
	for (i = 0; i < WALLS_POINTS; i++)
		walls->points[i] = get_point(i);
}

void walls_update(struct walls *walls)
{
	unsigned i;
	unsigned j;
	unsigned last;
	struct SDL_Point points[WALLS_POINTS];

	memcpy(points, walls->points, WALLS_POINTS * sizeof(*points));
	for (j = 0; j < 1 + SCREEN_HEIGHT / 3; j++) {
		SDL_SetRenderDrawColor(walls->renderer, BASE_LUMINANCE + 9 * j,
				BASE_LUMINANCE + 9 * j, BASE_LUMINANCE + 9 * j,
				SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLines(walls->renderer, points, WALLS_POINTS);
		for (i = 0; i < WALLS_POINTS; i++)
			points[i].y++;
	}
	for (i = 0; i < WALLS_POINTS; i++)
		walls->points[i].x--;

	if (walls->points[0].x <= -STEP) {
		last = WALLS_POINTS - 1;
		memmove(walls->points, walls->points + 1,
				last * sizeof(*walls->points));
		walls->points[last] = get_point(last);
		walls->points[last].x = walls->points[last - 1].x + STEP;
	}
}

void walls_cleanup(struct walls *walls)
{

}
