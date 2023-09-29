#ifndef CONTAINER_H
#define CONTAINTER_H

#include<stdio.h>
#include"enemy.h"
//#include"projectile.h"

// ENEMY PROJECTILES
enemies_container* enemies_container_create();
int enemies_container_add(enemies_container*, enemy_t*);
int enemies_container_remove(enemies_container*, int);
int enemies_container_clear(enemies_container*);
int enemies_container_rearrenge(enemies_container*);
int enemies_container_destroy(enemies_container**);
bool enemies_container_is_empty(enemies_container*);
void enemies_container_print(enemies_container*);

#endif
