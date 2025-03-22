#include"container.h"

array_list container_create(size_t type_size){

    array_list array = {
	.items = malloc(type_size * 2),
	.capacity = 2,
	.len = 0,
	.type_size = type_size,
    };

    memset(array.items, 0, type_size * 2);

    return array;
}



void container_add(array_list* array_list, void* element){
    if(array_list->len == array_list->capacity){
	array_list->capacity = array_list->capacity > 0 ? array_list->capacity * 2 : 1;
	void *items = realloc(array_list->items,
			      array_list->type_size * array_list->capacity);
	assert(items != NULL);

	array_list->items = items;
    }

    size_t index = array_list->len++;

    memcpy((uint8_t*)array_list->items + (index  * array_list->type_size),
	   element,
	   array_list->type_size);
}



void container_remove(array_list* array_list, size_t index){
    if(array_list->len == 0 || array_list->len <= index)
	return;

    --array_list->len;

    uint8_t *item_ptr = (uint8_t*)array_list->items + index * array_list->type_size;
    uint8_t *end_ptr = (uint8_t*)array_list->items + array_list->len * array_list->type_size;

    memcpy(item_ptr, end_ptr, array_list->type_size);
}



void *container_get(array_list* array_list, size_t index){
    if(array_list->len == 0)
	return NULL;

    return ((uint8_t*)array_list->items + (index * array_list->type_size));
}



int container_clear(array_list* array_list){
    free(array_list->items);
    return 0;
}



bool container_empty(array_list array_list){
    return (array_list.len == 0);
}



bool container_full(array_list array_list){
    return (array_list.len == array_list.capacity);
}
