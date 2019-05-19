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

#define INITIAL_PAUSE_DURATION 60

void message_init(struct message *message, struct SDL_Renderer *renderer,
		enum message_id id) {
	object_init(&message->object, renderer, &message_init_pos,
			*messages_list[id]);
	message->pause_duration = INITIAL_PAUSE_DURATION;
}

void message_update(struct message *message) {
	if (message_is_dead(message))
		return;
	object_render(&message->object);
	if (message->object.pos.x != 0)
		message->object.pos.x -= 2;
	else
		message->pause_duration--;
	if (message_is_dead(message))
		message_cleanup(message);
}

bool message_is_dead(const struct message *message) {
	return message->pause_duration == 0;
}

void message_cleanup(struct message *message) {
	object_cleanup(&message->object);
	memset(message, 0, sizeof(*message));
}
