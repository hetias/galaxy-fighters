#include"../include/container.h"

/**
 * CONTAINER_H
 * This container structure will work like a 
 * basic stack
 */

/**
 */
game_container container_create(int container_type){

    //check if typing is correct
    if(container_type != CONTAINER_ENEMY &&
       container_type != CONTAINER_PROJECTILE){
	printf("Desired type is unspecified\n");
	printf("Recived value = %d\n", container_type);
	printf("CONTAINER_ENEMY = %d\n", CONTAINER_ENEMY);
	printf("CONTAINER_PROJECTILE = %d\n", CONTAINER_PROJECTILE);

	game_container bad_container = {NULL, 0, 0, 0};
	return bad_container;        
    }

    //create the final container
    game_container container;

    //create the array
    if(container_type == CONTAINER_ENEMY){
    
	container.array = calloc(ENEMIES_CAP, sizeof(enemy_t*));
	container.size = ENEMIES_CAP;
    
    }else if(container_type == CONTAINER_PROJECTILE){

	container.array = calloc(PROJECTILES_CAP, sizeof(projectile_t*));
	container.size = PROJECTILES_CAP;
    }

    container.count = 0;
    container.type = container_type;

    return container;
}

/**
 * Here we infere the type of 'element' based on the container pased
 * and then add it at the end of the list
 */
int container_add(game_container* current_container, void* element){

    //Check if container pointer is valid
    if(current_container == NULL){
	printf("Pointer to container is null in 'container_add'\n");
	return -1;
    }  

    //Check if container pointer is valid
    if(element == NULL){
	printf("Pointer to element is null in 'container_add'\n");
	return -1;
    }

    //Check if it's not full
    if(container_full(*current_container)){
	printf("Failed on 'container_add' current container is full\n");
	return -1;
    }

    //if we need to add 'element' to an enemy container
    int last_index = (current_container->count);
    if(current_container->type == CONTAINER_ENEMY){
	current_container->array[last_index] = (enemy_t*)element;
	current_container->count += 1;
	//printf("Element added on position: %d\n", last_index);
    }  //if we need to add 'element' to an projectile container
    else if(current_container->type == CONTAINER_PROJECTILE){
	current_container->array[last_index] = (projectile_t*)element;
	current_container->count += 1;
	//printf("Element added on position: %d\n", last_index);
    }

    return 0;
}

/**
 * We swap the element selected with the last element(unless they are the same)
 * and then delete the last element
 */
int container_remove_destroy(game_container* current_container, int index){

    //check container existence
    if(current_container == NULL){
	printf("Invalid pointer on 'container_remove'\n");
	return -1;
    }

    //check if index is on bounds
    if(index < 0 || index > current_container->size){
	printf("Invalid index on 'container_remove'");
	return -1;
    }

    //check what we have to free
    if(current_container->type == CONTAINER_ENEMY){
	//check if index refers to the last element...
	int last_index = current_container->count - 1; //this is just the index of the last element

	if(index == last_index){
	    enemy_destroy((enemy_t**)(&current_container->array[last_index]));
	    current_container->count -= 1;
	}else{

	    //a little of swapping
	    enemy_t* tmp = current_container->array[index];      
	    current_container->array[index] = current_container->array[last_index];
	    current_container->array[last_index] = tmp;
	    //printf("Element from position %d swaped with element on: %d\n", index, last_index);
      
	    //now delete the element
	    enemy_destroy((enemy_t**)(&current_container->array[last_index]));
	    current_container->count -= 1;
      
	}
    }else if(current_container->type == CONTAINER_PROJECTILE){
	//check if index refers to the last element...
	int last_index = current_container->count - 1; //this is just the index of the last element

	if(index == last_index){
	    projectile_destroy((projectile_t**)(&current_container->array[last_index]));
	    current_container->count -= 1;
	}else{

	    //a little of swapping
	    projectile_t* tmp = current_container->array[index];      
	    current_container->array[index] = current_container->array[last_index];
	    current_container->array[last_index] = tmp;
	    //printf("Element from position %d swaped with element on: %d\n", index, last_index);

	    //now delete the element
	    projectile_destroy((projectile_t**)(&current_container->array[last_index]));
	    current_container->count -= 1;
	}
    }

    return 0;
}


void* container_remove(game_container* current_container, int index){

    //check container existence
    if(current_container == NULL){
	printf("Invalid pointer on 'container_remove'\n");
	return NULL;
    }

    //check if index is on bounds
    if(index < 0 || index > current_container->size){
	printf("Invalid index on 'container_remove'");
	return NULL;
    }

    //check what we have to remove
    if(current_container->type == CONTAINER_ENEMY){
	//check if index refers to the last element...
	int last_index = current_container->count - 1; //this is just the index of the last element

	if(index == last_index){
	    
	    //enemy_destroy((enemy_t**)(&current_container->array[last_index]));
	    current_container->count -= 1;
	    return (void*)(&current_container->array[last_index]);
	}else{

	    //a little of swapping
	    enemy_t* tmp = current_container->array[index];      
	    current_container->array[index] = current_container->array[last_index];
	    current_container->array[last_index] = tmp;
	    //printf("Element from position %d swaped with element on: %d\n", index, last_index);
      
	    //now delete the element
	    //enemy_destroy((enemy_t**)(&current_container->array[last_index]));
	    current_container->count -= 1;
	    return (void*)(&current_container->array[last_index]);
	}
    }else if(current_container->type == CONTAINER_PROJECTILE){
	//check if index refers to the last element...
	int last_index = current_container->count - 1; //this is just the index of the last element

	if(index == last_index){
	    //projectile_destroy((projectile_t**)(&current_container->array[last_index]));
	    current_container->count -= 1;
	    return (void*)(&current_container->array[last_index]);
	}else{

	    //a little of swapping
	    projectile_t* tmp = current_container->array[index];      
	    current_container->array[index] = current_container->array[last_index];
	    current_container->array[last_index] = tmp;
	    //printf("Element from position %d swaped with element on: %d\n", index, last_index);

	    //now delete the element
	    current_container->count -= 1;
	    return (void*)(&current_container->array[last_index]);
	}
    }

    return NULL;
}

/**
 */
int container_clear(game_container* current_container){

    //check if pointer is valid
    if(current_container == NULL){
	printf("Invalid pointer on container_clear\n");
	return RET_FAILURE;
    }

    //check if container's array is valid
    if(current_container->array == NULL){
	printf("Invalid array in container on container_clear\n");
	return RET_FAILURE;
    }

    //free each element in the array
    if(current_container->type == CONTAINER_ENEMY){

	for(int i = 0; i < current_container->count; i++){
	    enemy_t* enemy =  (enemy_t*)(current_container->array[i]);
	    enemy_destroy(&enemy);
	}
 
    }else if(current_container->type == CONTAINER_PROJECTILE){

	for(int i = 0; i < current_container->count; i++){
	    projectile_t* projectile = (projectile_t*)(current_container->array[i]);
	    projectile_destroy(&projectile);
	}
    
    }else{
	printf("Invalid container type found on container_clear\n");
	printf("Found value = %d\n", current_container->type);
    }
    //free array
    free(current_container->array);

    current_container->size = 0;
    current_container->count = 0;
    current_container->type = 0;

    return RET_SUCCESS;
}

/**
 */
bool container_empty(game_container current_container){
    return (current_container.count == 0);
}

/**
 */
bool container_full(game_container current_container){
    return (current_container.size == current_container.count);
}

/**
 */
int container_print(game_container current_container){

    printf("\n\n---------------------------------------\n");
    if(container_empty(current_container)){
	printf("Container currently empty\n");
    }

    printf("|");
    for(int i = 0; i < current_container.size; i++){
	if(current_container.array[i] == NULL){
	    printf("0|");
	}else{
	    printf("X|");
	}
    }
    printf("|\n");
    printf("size: %d - count: %d\n", current_container.size, current_container.count);
}


