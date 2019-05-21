#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <stdbool.h>

#include "object.h"

enum message_id {
	MESSAGE_ID_LOOSER,

	MESSAGE_ID_READY,
	MESSAGE_ID_SET,
	MESSAGE_ID_GO,

	MESSAGE_ID_AIM_DAMMIT,
	MESSAGE_ID_WAKE_UP,
	MESSAGE_ID_MUAHAHA,
	MESSAGE_ID_OPEN_YOUR_EYES,
	MESSAGE_ID_ARE_YOU_BLIND,
	MESSAGE_ID_TOO_BAD,
	MESSAGE_ID_ARE_YOU_DRUNK,
	MESSAGE_ID_CHICKEN,
	MESSAGE_ID_ALIEN_LAUGHING,

	MESSAGE_ID_NOT_THAT_BAD,
	MESSAGE_ID_WELL_DONE,
	MESSAGE_ID_GOOD,
	MESSAGE_ID_BORN_TO_KILL,
	MESSAGE_ID_LUCKY_I_M_NOT_AN_ALIEN,
	MESSAGE_ID_KILLING_MACHINE,
	MESSAGE_ID_SNIPER,
	MESSAGE_ID_UNSTOPPABLE,
	MESSAGE_ID_ALIEN_SLAUGHTERER,
	MESSAGE_ID_LIKE_A_GOD,

};

struct message {
	struct object object;
	unsigned pause_duration;
	enum message_id id;
};

void message_init(struct message *message, struct SDL_Renderer *renderer,
		enum message_id id);
void message_update(struct message *message);
bool message_is_dead(const struct message *message);
enum message_id message_random_taunt(void);
void message_cleanup(struct message *message);

#endif /* MESSAGE_H_ */
