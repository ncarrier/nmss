nmss:main.c
	gcc main.c -o main `pkg-config sdl2 SDL2_image --cflags --libs` \
		-Werror -Wall -Wextra -Wno-unused-parameter
