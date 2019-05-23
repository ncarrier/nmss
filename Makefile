WARNINGS := -Wall -Wextra -Wno-unused-parameter
OPTIMIZATION := -O3

all: nmss nmss.exe nmss.html

nmss:*.c *.h res/*.xpm
	gcc *.c -o nmss `pkg-config sdl2 SDL2_image --cflags --libs` -lm \
		$(WARNINGS) $(OPTIMIZATION)

nmss.exe:*.c *.h
	x86_64-w64-mingw32-gcc *.c -o nmss.exe \
		-Iwin_root/include/ -Drandom=rand -DSDL_MAIN_HANDLED \
		-lSDL2 -lSDL2_image -L win_root/lib/ \
		$(WARNINGS) $(OPTIMIZATION)

nmss.html:*.c *.h res/*.xpm
	emcc *.c -o $@ -lm $(WARNINGS) $(OPTIMIZATION) \
		-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_LIBPNG=1 \
		-s USE_LIBJPEG=1 -s SDL2_IMAGE_FORMATS='["xpm"]'

clean:
	rm -f nmss nmss.js nmss.html nmss.exe

.PHONY: clean
