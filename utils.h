#ifndef UTILS_H_
#define UTILS_H_

#include <SDL2/SDL.h>

/* GCC is awesome. */
#define ARRAY_SIZE(arr) \
	(sizeof(arr) / sizeof((arr)[0]) \
		+ sizeof(typeof(int[1 - 2 * \
		!!__builtin_types_compatible_p(typeof(arr), \
		typeof(&arr[0]))])) * 0)

void sdl_surface_cleanup(struct SDL_Surface **surface);

typedef char *image[];

#endif /* UTILS_H_ */
