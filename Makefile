#OBJS specifies which files to compile as part of the project
OBJS = main.o

#CC specifies used compiler
CC = g++

#INCLUDE_PATHS specifies additional include paths we'll need
INCLUDE_PATHS = -IC:\libraries\sdl2\include

#LIBRARY_PATHS specifies additional libraries
LIBRARY_PATHS = -LC:\libraries\sdl2\lib

#COMPILER_FLAGS specifies additional compilitaion options
COMPILER_FLAGS = -w -Wl, -subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking to
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -std=c++14

#OBJ_NAME = specifies the name of the executable
OBJ_NAME = main

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)

main.o: main.cpp
	$(CC) $(INCLUDE_PATHS) -c main.cpp
