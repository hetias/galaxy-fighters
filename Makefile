OBJS = main.o container.o enemy.o player.o projectile.o scene.o spline.o ui.o keyframe.o gg.o

CC = gcc

INCLUDE_PATHS = -I`sdl2-config --cflags`

LIBRARY_PATHS = -L`sdl2-config --static-libs`

COMPILER_FLAGS = -Wall -Wextra

LINKER_FLAGS = -lSDL2main `sdl2-config --static-libs` -lSDL2_image -lSDL2_ttf

OBJ_NAME = main

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

main.o: source/main.c
	$(CC) $(INCLUDE_PATHS) -g -c source/main.c

gg.o: source/gg.h source/gg.c
	$(CC) $(INCLUDE_PATHS) -g -c source/gg.c

ui.o: source/core/ui.h source/core/ui.c
	$(CC) $(INCLUDE_PATHS) -g -c source/core/ui.c

container.o: source/core/container.h source/core/container.c
	$(CC) $(INCLUDE_PATHS) -g -c source/core/container.c

enemy.o: source/enemy.h source/enemy.c
	$(CC) $(INCLUDE_PATHS) -g -c source/enemy.c

player.o: source/player.h source/player.c
	$(CC) $(INCLUDE_PATHS) -g -c source/player.c

projectile.o: source/projectile.h source/projectile.c
	$(CC) $(INCLUDE_PATHS) -g -c source/projectile.c

keyframe.o: source/keyframe.h source/keyframe.c
	$(CC) $(INCLUDE_PATHS) -g -c source/keyframe.c

scene.o: source/scene.h source/scene.c
	$(CC) $(INCLUDE_PATHS) -g -c source/scene.c

spline.o: source/spline.h source/spline.c
	$(CC) $(INCLUDE_PATHS) -g -c source/spline.c

clean:
	rm *.o
