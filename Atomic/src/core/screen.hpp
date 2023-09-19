#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <xcb/xcb.h>
#include <vector>
#include "core/math.hpp"

#include "handling.hpp"
#include "offsets/offsets.cpp"

struct ScreenSize {
    int width;
    int height;
};

struct ScreenMidpoint {
    int x;
    int y;
};

struct PlayerInfo {
    float height;
    float width;
    float middleX;
    float middleY;
    float headX;
    float headY;
};

struct PlayerData {
    float height;
    float width;
    float middleX;
    float middleY;
};


ScreenSize getScreenSize();
ScreenMidpoint getScreenMidpoint();
vec3 worldToScreen(const vec3 pos, Matrix matrix);

PlayerData calculatePlayerData(vec3 w2sEntHead, vec3 w2sEntFeet);

float vec_WTSdistance(vec3 v1, vec3 v2);

#endif // SCREEN_HPP
