CC	= gcc
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LDFLAGS := $(shell sdl2-config --libs)
CFLAGS = -O3 -Wall -Winline -pipe -std=gnu99 $(SDL2_CFLAGS) -I./src/include
LDLIBS = $(SDL2_LDFLAGS) -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lm

OBJS =  spang.o balls.o combo.o overlay.o player.o powerup.o sdl.o bullets.o explosion.o bullets.o enemy.o detect.o 
OBJS += levels.o level.o resources.o hiscore.o bg.o hsentry.o game.o amode.o bonus.o

spang: $(OBJS)
	@echo [link]
	@$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	@echo "[Removing object files]"
	@find . -name '*.o' -delete
	@echo "[Removing binaries]"
	@rm -f spang
