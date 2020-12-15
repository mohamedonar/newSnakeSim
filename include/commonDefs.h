#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <Arduino.h>

enum DIRECTION {dRIGHT, dLEFT, dUP, dDOWN};

struct segment {
    DIRECTION   dir;
    int         nDirCoord;
    int         nPmin;
    int         nPmax;
};

struct pixel {
    int8_t x;
    int8_t y;
};

#endif
