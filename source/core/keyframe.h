#ifndef KEYFRAME_H
#define KEYFRAME_H

enum keyframe_action{
    KEYFRAME_ENEMY_ADD,
    KEYFRAME_ENEMY_CHANGE_PATH,
    KEYFRAME_ENEMY_DESTROY
};

typedef struct{
    int id;           //used by KEYFRAME_ENEMY_DESTROY KEYFRAME_ENEMY_CHANGE_PATH
    int enemy_type;   //used by KEYFRAME_ENEMY_ADD
    int pathid;   //used by KEYFRAME_ENEMY_ADD KEYFRAME_ENEMY_CHANGE_PATH
}keyframe_params;

typedef struct{
    int tick;
    int  action;
    keyframe_params params;
}keyframe_t;

/*int tick - int enemy_type - int path_id*/
keyframe_t keyframe_create_enemy(int, int, int);

/*int tick - int enemy_id*/
keyframe_t keyframe_destroy_enemy(int, int);

/*int tick - enemy_id - path_id*/
keyframe_t keyframe_change_enemy_path(int, int, int);

#endif

/**
 * LEVEL.LV*
 PATHS
 INT
 ...
 KEYFRAMES
 ...
**/
