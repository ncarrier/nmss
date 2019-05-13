nmss:*.c *.h
	gcc *.c -o nmss `pkg-config sdl2 SDL2_image --cflags --libs` -lm \
		-Werror -Wall -Wextra -Wno-unused-parameter -O3
