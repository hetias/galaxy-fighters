OBJS = main.o container.o enemy.o player.o projectile.o scene.o spline.o ui.o keyframe.o

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

ui.o: source/include/ui.h source/lib/ui.c
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/ui.c

container.o: source/include/container.h source/lib/container.c
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/container.c

enemy.o: source/include/enemy.h source/lib/enemy.c
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/enemy.c

player.o: source/include/player.h source/lib/player.c
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/player.c

projectile.o: source/include/projectile.h source/lib/projectile.c
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/projectile.c

keyframe.o: source/include/keyframe.h source/lib/keyframe.c
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/keyframe.c

scene.o: source/include/scene.h source/lib/scene.c
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/scene.c

spline.o: source/include/spline.h source/lib/spline.c
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/spline.c

clean:
	rm *.o
