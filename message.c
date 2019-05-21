#include <stdio.h>

#include "message.h"
#include "messages_list.h"
#include "screen.h"

static const struct SDL_Rect message_init_pos = {
		.x = SCREEN_WIDTH,
		.y = 0,
		.w = SCREEN_WIDTH,
		.h = SCREEN_HEIGHT
};

#define INITIAL_PAUSE_DURATION 30

void message_init(struct message *message, struct SDL_Renderer *renderer,
		enum message_id id) {
	object_init(&message->object, renderer, &message_init_pos,
			*messages_list[id]);
	message->pause_duration = INITIAL_PAUSE_DURATION;
	message->id = id;
}

void message_update(struct message *message) {
	enum message_id id;
	struct SDL_Renderer *renderer;

	if (message_is_dead(message))
		return;
	object_render(&message->object);
	if (message->object.pos.x > 0)
		message->object.pos.x -= 3;
	else
		message->pause_duration--;
	if (message_is_dead(message)) {
		id = message->id;
		if (id == MESSAGE_ID_READY || id == MESSAGE_ID_SET) {
			id++;
			renderer = message->object.renderer;
			message_cleanup(message);
			message_init(message, renderer, id);
			message->object.pos.x -= 70;
		}
	}
}

bool message_is_dead(const struct message *message) {
	return message->pause_duration == 0;
}

enum message_id message_random_taunt(void)
{
	return random_in_range(MESSAGE_ID_AIM_DAMMIT, MESSAGE_ID_ALIEN_LAUGHING);
}

void message_cleanup(struct message *message) {
	object_cleanup(&message->object);
	memset(message, 0, sizeof(*message));
}
