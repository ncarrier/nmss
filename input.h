/*
 * input.h
 *
 *  Created on: 11 mai 2019
 *      Author: nicolas
 */

#ifndef INPUT_H_
#define INPUT_H_
#include <stdbool.h>

#include <SDL2/SDL_events.h>

struct input {
	bool loop;
	bool pause;
	bool a_down;
	bool b_down;
	bool down_down;
	bool left_down;
	bool right_down;
	bool select_down;
	bool start_down;
	bool up_down;
	bool fullscreen;
	union SDL_Event event;
	struct SDL_Window *window;
};

void input_init(struct input *input, struct SDL_Window *window);
void input_update(struct input *input);
void input_cleanup(struct input *input);

#endif /* INPUT_H_ */
