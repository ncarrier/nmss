WARNINGS := -Werror -Wall -Wextra -Wno-unused-parameter
OPTIMIZATION := -O3
nmss:*.c *.h res/*.xpm
	gcc *.c -o nmss `pkg-config sdl2 SDL2_image --cflags --libs` -lm \
		$(WARNINGS) $(OPTIMIZATION)

nmss.html:*.c *.h res/*.xpm
	emcc *.c -o $@ -lm $(WARNINGS) $(OPTIMIZATION) \
		-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_LIBPNG=1 \
		-s USE_LIBJPEG=1 -s SDL2_IMAGE_FORMATS='["xpm"]'

clean:
	rm -f nmss nmss.js nmss.html

.PHONY: clean
