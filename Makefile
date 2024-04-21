GAME_OBJS = game.o container.o enemy.o player.o projectile.o scene.o spline.o ui.o keyframe.o gg.o timing.o

ENGINE_OBJS = engine.o spline.o keyframe.o gg.o

CC = gcc

INCLUDE_PATHS = -I

LIBRARY_PATHS = -L

COMPILER_FLAGS = -Wall -Wextra

LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lm

#BUILDING ACTUAL GAME APP
game: game.o
	$(CC) $(GAME_OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o game

#BUILDING THE ENGINE
engine: engine.o
	$(CC) $(ENGINE_OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o engine

#OBJS
engine.o: source/engine.c gg.o keyframe.o spline.o timing.o
	$(CC) $(INCLUDE_PATHS) -g -c source/engine.c

game.o: source/game.c gg.o ui.o container.o enemy.o player.o projectile.o keyframe.o scene.o spline.o timing.o
	$(CC) $(INCLUDE_PATHS) -g -c source/game.c

gg.o: source/core/gg.h source/core/gg.c
	$(CC) $(INCLUDE_PATHS) -g -c source/core/gg.c

ui.o: source/core/ui.h source/core/ui.c
	$(CC) $(INCLUDE_PATHS) -g -c source/core/ui.c

container.o: source/core/container.h source/core/container.c
	$(CC) $(INCLUDE_PATHS) -g -c source/core/container.c

enemy.o: source/enemy.c player.o projectile.o
	$(CC) $(INCLUDE_PATHS) -g -c source/enemy.c

player.o: source/player.c projectile.o
	$(CC) $(INCLUDE_PATHS) -g -c source/player.c

projectile.o: source/projectile.h source/projectile.c
	$(CC) $(INCLUDE_PATHS) -g -c source/projectile.c

keyframe.o: source/core/keyframe.h source/core/keyframe.c
	$(CC) $(INCLUDE_PATHS) -g -c source/core/keyframe.c

scene.o: source/core/scene.c source/core/definitions.h keyframe.o
	$(CC) $(INCLUDE_PATHS) -g -c source/core/scene.c

spline.o: source/core/spline.h source/core/spline.c
	$(CC) $(INCLUDE_PATHS) -g -c source/core/spline.c

timing.o: source/core/timing.h source/core/timing.c
	$(CC) $(INCLUDE_PATHS) -g -c source/core/timing.c

clean:
	rm *.o
