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
    spline_t *path;   //used by KEYFRAME_ENEMY_ADD     KEYFRAME_ENEMY_CHANGE_PATH
}keyframe_params;

typedef struct{
    int tick;
    int  action;
    keyframe_params params;
}keyframe_t;

#endif
