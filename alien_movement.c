#include <stdio.h>
#include <math.h>

#include "alien_movement.h"
#include "utils.h"
#include "screen.h"

static int mvt_x_0(const struct alien_movement *mvt, int x, int y) {
	return x - 1;
}

static int mvt_y_0(const struct alien_movement *mvt, int x, int y) {
	return cos(mvt->t * 0.1) * (SCREEN_HEIGHT / 3.) + (SCREEN_HEIGHT / 2);
}

static int mvt_x_1(const struct alien_movement *mvt, int x, int y) {
	return x - 1;
}

static int mvt_y_1(const struct alien_movement *mvt, int x, int y) {
	return y;
}

/* template movements for easy implementation of _init */
static const struct alien_movement movements[] = {
	{
		.mvt_x = mvt_x_0,
		.mvt_y = mvt_y_0,
	},
	{
		.mvt_x = mvt_x_1,
		.mvt_y = mvt_y_1,
	},
};

void alien_movement_init(struct alien_movement *mvt, unsigned movement_index) {
	*mvt = movements[movement_index];
}

unsigned alien_movement_get_nb(void) {
	return ARRAY_SIZE(movements);
}

void alien_movement_update(struct alien_movement *mvt) {
	mvt->t++;
}

int alien_movement_get_x(struct alien_movement *mvt, int x, int y) {
	return mvt->mvt_x(mvt, x, y);
}

int alien_movement_get_y(struct alien_movement *mvt, int x, int y) {
	int result;

	result = mvt->mvt_y(mvt, x, y);
	if (result < 0)
		return 0;
	if (result > SCREEN_HEIGHT - 8)
		return SCREEN_HEIGHT - 8;

	return result;
}
