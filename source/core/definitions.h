#pragma once

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

//definitions needed for player and enemy

//this has to be divided by 60 cause of vsync
#define SHOOT_SLOW   375 / 60
#define SHOOT_NORMAL 200 / 60
#define SHOOT_FAST   155 / 60

#define DEBUG 1

enum ETextures{
    //background
    TXT_BG_BLACK,
    TXT_BG_PURPLE,
    TXT_BG_BLUE,
    TXT_BG_DARKPURPLE,
    //player
    TXT_PLAYER_BLUE,
    //enemy
    TXT_ENEMY1_BLACK,
    //lasers
    TXT_LASER_BLUE,
    TXT_LASER_RED,
    //ui
    TXT_UI_LIFE,
    //vector final size
    TXT_TOTAL
};
