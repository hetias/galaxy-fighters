#ifndef CONTAINER_H
#define CONTAINER_H

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#include<inttypes.h>
#include<stddef.h>
#include<stdbool.h>
#include<string.h>

typedef struct{
    void** items;
    size_t capacity;
    size_t len;
    size_t  type_size;
}array_list;

array_list container_create(size_t);

void container_add(array_list*, void*);
void container_remove(array_list*, size_t);
void *container_get(array_list*, size_t index);

int container_clear(array_list*);

bool container_empty(array_list);
bool container_full(array_list);

#endif
