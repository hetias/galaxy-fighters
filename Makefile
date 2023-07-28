#OBJS specifies which files to compile as part of the project
OBJS = main.o player.o

#CC specifies used compiler
CC = g++

#INCLUDE_PATHS specifies additional include paths we'll need
INCLUDE_PATHS = -I`sdl2-config --cflags`

#LIBRARY_PATHS specifies additional libraries
LIBRARY_PATHS = -L

#COMPILER_FLAGS specifies additional compilitaion options
COMPILER_FLAGS = 

#LINKER_FLAGS specifies the libraries we're linking to
LINKER_FLAGS = -lSDL2main `sdl2-config --static-libs` -lSDL2_image -lSDL2_ttf -std=c++14

#OBJ_NAME = specifies the name of the executable
OBJ_NAME = main

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)

main.o: main.cpp
	$(CC) $(INCLUDE_PATHS) -c main.cpp

player.o: player.hpp player.cpp
	$(CC) $(INCLUDE_PATH) -c player.cpp