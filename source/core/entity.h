#ifndef ENTITY_H
#define ENTITY_H

typedef struct entity_h{
    SDL_FPoint pos;
    SDL_FPoint dir;
    SDL_Frect hitbox;
    int hp;
    float speed;
    SDL_Texture* sprite;
}entity_t;

typedef struct player_s{
    entity_t ent;
    int shootDelay;
    int currentShootDelay;
};

typedef struct enemy_s{
    entity_t ent;
    spline_t *path;
    PATH_STATE path_state;
    PATH_TYPE path_type;
}

typedef struct projectile_s{
    entity_t ent;
    bool is_friendly;
}projectile_t;

#endif
