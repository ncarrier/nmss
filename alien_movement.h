#ifndef ALIEN_MOVEMENT_H_
#define ALIEN_MOVEMENT_H_

struct alien_movement;
typedef int (*alien_movement_fn)(struct alien_movement *mvt, int x, int y);

struct alien_movement {
	int t;
	alien_movement_fn mvt_x;
	alien_movement_fn mvt_y;
	unsigned alien_index;
	int data;
};

void alien_movement_init(struct alien_movement *mvt, unsigned movement_index,
		unsigned alien_index);
unsigned alien_movement_get_nb(void);
void alien_movement_update(struct alien_movement *mvt);
int alien_movement_get_x(struct alien_movement *mvt, int x, int y);
int alien_movement_get_y(struct alien_movement *mvt, int x, int y);

#endif /* ALIEN_MOVEMENT_H_ */
