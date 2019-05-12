#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "alien_movement.h"
#include "utils.h"
#include "screen.h"

static int mvt_x_0(struct alien_movement *mvt, int x, int y) {
	return x - 1;
}

static int mvt_y_0(struct alien_movement *mvt, int x, int y) {
	return y;
}

static int mvt_x_1(struct alien_movement *mvt, int x, int y) {
	return x - 1;
}

static int mvt_y_1(struct alien_movement *mvt, int x, int y) {
	return cos((mvt->t + mvt->alien_index) * 0.1) * (SCREEN_HEIGHT / 3.) + (SCREEN_HEIGHT / 2);
}

static int mvt_x_2(struct alien_movement *mvt, int x, int y) {
	return x - 1 + (random() % 3) - 1;
}

static int mvt_y_2(struct alien_movement *mvt, int x, int y) {
	if (mvt->t == 0)
		return random() % (SCREEN_HEIGHT - 8);

	return y + (random() % 5) - 2;
}

static int mvt_x_3(struct alien_movement *mvt, int x, int y) {
	return x - 1 + (random() % 3) - 1;
}

static int mvt_y_3(struct alien_movement *mvt, int x, int y) {
	return cos((mvt->t + (random() % 3) - 1) * 0.1) *
		(SCREEN_HEIGHT / 3.) + (SCREEN_HEIGHT / 2);
}

static int mvt_x_4(struct alien_movement *mvt, int x, int y) {
	return sin(mvt->t * 0.1) * (SCREEN_HEIGHT / 3.) + (SCREEN_WIDTH) - mvt->t;
}

static int mvt_y_4(struct alien_movement *mvt, int x, int y) {
	return cos(mvt->t * 0.1) * (SCREEN_HEIGHT / 3.) + (SCREEN_HEIGHT / 2);
}

static int mvt_x_5(struct alien_movement *mvt, int x, int y) {
	return x - 1;
}

static int mvt_y_5(struct alien_movement *mvt, int x, int y) {
	return (mvt->alien_index * SCREEN_HEIGHT / 5) % (SCREEN_HEIGHT - 8);
}

static int mvt_x_6(struct alien_movement *mvt, int x, int y) {
	int increment[] = {2, 0, -1, 0};
	int phase;

	phase = (mvt->t / 30) % 4;

	return x - increment[phase];
}

static int mvt_y_6(struct alien_movement *mvt, int x, int y) {
	int increment[] = {0, 1, 0, -1};
	int phase;

	phase = (mvt->t / 30) % 4;

	if (mvt->t == 0)
		return random() % (SCREEN_HEIGHT - 8);

	return y - increment[phase];
}

static int mvt_x_7(struct alien_movement *mvt, int x, int y) {
	return x - 1;
}

static int mvt_y_7(struct alien_movement *mvt, int x, int y) {
	if (mvt->t % 40 == 0)
		return random() % (SCREEN_HEIGHT - 8);

	return y;
}

static int mvt_x_8(struct alien_movement *mvt, int x, int y) {
	return x - 1;
}

static int mvt_y_8(struct alien_movement *mvt, int x, int y) {
	int result;

	if (mvt->t == 0)
		return random() % (SCREEN_HEIGHT - 8);

	if (mvt->t % 40 == 0)
		mvt->data = (random() % 3) - 1;

	result = y + mvt->data;
	if (result <= 0)
		mvt->data = 1;
	if (result >= SCREEN_HEIGHT - 8)
		mvt->data = -1;

	return result;
}

static int mvt_x_9(struct alien_movement *mvt, int x, int y) {
	return x - cos((mvt->t + mvt->alien_index) * .1) - 1;
}

static int mvt_y_9(struct alien_movement *mvt, int x, int y) {
	if (mvt->t == 0)
		return random() % (SCREEN_HEIGHT - 8);

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
	{
		.mvt_x = mvt_x_2,
		.mvt_y = mvt_y_2,
	},
	{
		.mvt_x = mvt_x_3,
		.mvt_y = mvt_y_3,
	},
	{
		.mvt_x = mvt_x_4,
		.mvt_y = mvt_y_4,
	},
	{
		.mvt_x = mvt_x_5,
		.mvt_y = mvt_y_5,
	},
	{
		.mvt_x = mvt_x_6,
		.mvt_y = mvt_y_6,
	},
	{
		.mvt_x = mvt_x_7,
		.mvt_y = mvt_y_7,
	},
	{
		.mvt_x = mvt_x_8,
		.mvt_y = mvt_y_8,
	},
	{
		.mvt_x = mvt_x_9,
		.mvt_y = mvt_y_9,
	},
};

void alien_movement_init(struct alien_movement *mvt, unsigned movement_index,
		unsigned alien_index) {
	*mvt = movements[movement_index];
	mvt->alien_index = alien_index;
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
