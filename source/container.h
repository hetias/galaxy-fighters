#ifndef CONTAINER_H
#define CONTAINER_H

#include<stdio.h>
#include<stdlib.h>

typedef struct{
  void** array;
  int size;
  int count;
  int  type;
}game_container;

#include"enemy.h"
#include"projectile.h"

#define ENEMIES_CAP 32
#define PROJECTILES_CAP 128

enum CONTAINER_TYPE{CONTAINER_ENEMY = 1, CONTAINER_PROJECTILE = 2};

game_container container_create(int);
int container_add(game_container*, void*);
void* container_remove(game_container*, int);
int container_remove_destroy(game_container*, int);
int container_clear(game_container*);
bool container_empty(game_container);
bool container_full(game_container);
int container_print(game_container);

#endif
