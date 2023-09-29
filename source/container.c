#include"container.h"

/**
 */
game_container container_create(size_t container_size, int container_type){

  //check if size it's reasonable
  if(container_size > MAX_CONTAINER){
    printf("Container size exceds MAX_CONTAINER limit\n");
    printf("MAX_CONTAINER = %d", MAX_CONTAINER);
    printf("Desired size = %ld", container_size);

    game_container bad_container = {NULL, 0, 0, 0};
    return bad_container;        
  }

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
    
    container.array = calloc(container_size, sizeof(enemy_t*));
     
  }else if(container_type == CONTAINER_PROJECTILE){

    container.array = calloc(container_size, sizeof(projectile_t*));
    
  }

  container.size = container_size;
  container.count = 0;
  container.type = container_type;

  return container;
}

/**
 */
int container_add(game_container* current_container, void* element){
  
}

/**
*/
int container_remove(game_container* current_container, int index){
  
}

/**
 */
int container_clear(game_container* current_container){

  //check if pointer is valid
  if(current_container == NULL){
    printf("Invalid pointer on container_clear\n");
    return -1;
  }

  //check if container's array is valid
  if(current_container->array == NULL){
    printf("Invalid array in container on container_clear\n");
    return -1;
  }

  //free each element in the array

  if(current_container->type == CONTAINER_ENEMY){

    for(int i = 0; i < current_container->size; i++){
      enemy_destroy((enemy_t*)current_container->array[i]);
    }
 
  }else if(current_container->type == CONTAINER_PROJECTILE){

    for(int i = 0; i < current_container->size; i++){
      projectile_t* prj = (projectile_t*)current_container->array[i];
      projectile_destroy(&prj);
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

  return 0;
}

/**
 */
int container_rearrenge(game_container* current_container){
  
}

/**
 */
int container_empty(game_container current_container){
  
}

/**
 */
int container_full(game_container current_container){
  
}

/**
 */
int container_print(game_container current_container){
  
}


