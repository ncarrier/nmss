#include "explosion.h"
#include "object.h"

#include "res/explode_1.xpm"
#include "res/explode_2.xpm"
#include "res/explode_3.xpm"

void explosion_init(struct explosion *explosion,
		struct SDL_Renderer *renderer) {
	object_init(&explosion->explode_1, renderer, NULL, explode_1_xpm);
	object_init(&explosion->explode_2, renderer, NULL, explode_2_xpm);
	object_init(&explosion->explode_3, renderer, NULL, explode_3_xpm);
	explosion->phase = 0;
	explosion->sound = Mix_LoadWAV("res/sfx/explode.wav");
}

void explosion_update(struct explosion *explosion) {
	if (explosion->phase == 0)
		return;

	switch (explosion->phase) {
	case 1 ... 20:
		object_render(&explosion->explode_3);
		break;

	case 21 ... 40:
		object_render(&explosion->explode_2);
		break;

	case 41 ... 60:
		object_render(&explosion->explode_1);
		break;
	}
	explosion->phase--;
}

void explosion_start(struct explosion *explosion, const struct SDL_Rect *pos) {
	object_set_pos(&explosion->explode_1, pos);
	object_set_pos(&explosion->explode_2, pos);
	object_set_pos(&explosion->explode_3, pos);
	explosion->phase = 60;
	Mix_PlayChannel(2, explosion->sound, 0);
}

bool explosion_is_dead(const struct explosion *explosion) {
	return explosion->phase == 0;
}

void explosion_cleanup(struct explosion *explosion)
{
	Mix_FreeChunk(explosion->sound);
	object_cleanup(&explosion->explode_1);
	object_cleanup(&explosion->explode_2);
	object_cleanup(&explosion->explode_3);
}
