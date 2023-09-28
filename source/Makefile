#OBJS specifies which files to compile as part of the project
OBJS = main.o scene.o player.o

#projectile.o player.o enemy.o spline.o scene.o

#CC specifies used compiler
CC = gcc

#INCLUDE_PATHS specifies additional include paths we'll need
INCLUDE_PATHS = -I`sdl2-config --cflags`

#LIBRARY_PATHS specifies additional libraries
LIBRARY_PATHS = -L

#COMPILER_FLAGS specifies additional compilitaion options
COMPILER_FLAGS = -Wall -Wextra

#LINKER_FLAGS specifies the libraries we're linking to
LINKER_FLAGS = -lSDL2main `sdl2-config --static-libs` -lSDL2_image -lSDL2_ttf

#OBJ_NAME = specifies the name of the executable
OBJ_NAME = main

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

main.o: main.c
	$(CC) $(INCLUDE_PATHS) -g -c main.c

# projectile.o: projectile.h projectile.c
# 	$(CC) $(INCLUDE_PATH)  -g -O0 -c projectile.c

player.o: player.h player.c
	$(CC) $(INCLUDE_PATH) -g -O0 -c player.c

# enemy.o: enemy.h enemy.c
# 	$(CC) $(INCLUDE_PATH) -g -O0 -c enemy.c

# spline.o: spline.h spline.c
# 	$(CC) $(INCLUDE_PATH) -g -O0 -c spline.c

scene.o: scene.c scene.h
	$(CC) $(INCLUDE_PATH) -g -O0 -c scene.c

clean:
	rm *.o
