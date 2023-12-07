OBJS = main.o container.o enemy.o player.o projectile.o scene.o spline.o ui.o

CC = gcc

INCLUDE_PATHS = -I`sdl2-config --cflags`

LIBRARY_PATHS = -L

COMPILER_FLAGS = -Wall -Wextra

LINKER_FLAGS = -lSDL2main `sdl2-config --static-libs` -lSDL2_image -lSDL2_ttf -std=c++14

OBJ_NAME = main

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

build/main.o: source/main.c
	$(CC) $(INCLUDE_PATHS) -g -c source/main.c

ui.o: source/include/ui.h
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/ui.c

container.o: source/include/container.h
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/container.c

enemy.o: source/include/enemy.h
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/enemy.c

player.o: source/include/player.h
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/player.c

projectile.o: source/include/projectile.h
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/projectile.c

scene.o: source/include/scene.h
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/scene.c

spline.o: source/include/spline.h
	$(CC) $(INCLUDE_PATHS) -g -c source/lib/spline.c

clean:
	rm *.o
