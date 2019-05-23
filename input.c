#include <stdio.h>

#include "input.h"

void input_init(struct input *input)
{
	*input = (struct input) {
		.loop = true,
		.a_down = false,
		.b_down = false,
		.down_down = false,
		.left_down = false,
		.right_down = false,
		.select_down = false,
		.start_down = false,
		.up_down = false,
	};
}

static void handle_key(struct input *input, bool down)
{
	switch (input->event.key.keysym.sym) {
	case SDLK_a:
		input->a_down = down;
		break;

	case SDLK_b:
		input->b_down = down;
		break;

	case SDLK_DOWN:
		input->down_down = down;
		break;

	case SDLK_LEFT:
		input->left_down = down;
		break;

	case SDLK_RIGHT:
		input->right_down = down;
		break;

	case SDLK_SPACE:
		if (down)
			input->pause = !input->pause;
		break;

	case SDLK_UP:
		input->up_down = down;
		break;
	}
}

void input_update(struct input *input)
{
	while (true) {
		if (SDL_PollEvent(&input->event) == 0)
			return;

		switch (input->event.type) {
		case SDL_QUIT: {
			printf("see u.\n");
			input->loop = false;
			break;
		}

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			handle_key(input, input->event.type == SDL_KEYDOWN);
			break;
		}
	}
}

void input_cleanup(struct input *input)
{

}

