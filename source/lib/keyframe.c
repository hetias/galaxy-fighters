#include"../include/keyframe.h"

keyframe_t keyframe_create_enemy(int tick, int enemy_type, int path_id){
    return (keyframe_t){tick, KEYFRAME_ENEMY_ADD, (keyframe_params){-1, enemy_type, path_id}};
}

keyframe_t keyframe_destroy_enemy(int tick, int id){
    return (keyframe_t){tick, KEYFRAME_ENEMY_DESTROY, (keyframe_params){id, -1, -1}};
}

keyframe_t keyframe_change_enemy_path(int tick, int enemy_id, int path_id){
    return (keyframe_t){tick, KEYFRAME_ENEMY_CHANGE_PATH, (keyframe_params){enemy_id, -1, path_id}};
}
