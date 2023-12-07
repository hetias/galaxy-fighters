#ifndef KEYFRAME_H
#define KEYFRAME_H

enum keyframe_action{
    KEYFRAME_ENEMY_ADD,
    KEYFRAME_ENEMY_CHANGE_PATH,
    KEYFRAME_ENEMY_DESTROY
};

typedef struct{
    int id;
    enemy_t enemy;
    spline_t path;
}keyframe_params;

typedef struct{
    int tick;
    int  action;
    int param_total;
    void* parameters;
}keyframe_t;

#endif
