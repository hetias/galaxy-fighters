#ifndef CONTAINER_H
#define CONTAINER_H

#include<stdio.h>
#include<stdlib.h>
#include"enemy.h"
#include"projectile.h"

#define MAX_CONTAINER 128

enum CONTAINER_TYPE{CONTAINER_ENEMY = 1, CONTAINER_PROJECTILE = 2};

typedef struct{
  void** array;
  int size;
  int count;
  int  type;
}game_container;

game_container container_create(size_t, int);
int container_add(game_container*, void*);
int container_remove(game_container*, int);
int container_clear(game_container*);
int container_rearrenge(game_container*);
bool container_empty(game_container);
bool container_full(game_container);
int container_print(game_container);

#endif
