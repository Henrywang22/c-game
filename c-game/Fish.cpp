#include "Fish.h"
#include <cstdlib>
#include <cmath>

Fish::Fish(int x, int y, Type type)
    : x(x), y(y), type(type)
{
    if (type == EDIBLE) {
        value = 10 + rand() % 20;
        staminaGain = 20;
        vx = (rand() % 3) - 1;
        vy = (rand() % 3) - 1;
        catchRequired = 3;   // 按3次
        catchTimeLimit = 180; // 3秒（180帧）
    }
    else {
        value = 50 + rand() % 80;
        staminaGain = 8;
        vx = (rand() % 5) - 2;
        vy = (rand() % 5) - 2;
        catchRequired = 8;   // 按8次
        catchTimeLimit = 90; // 1.5秒（90帧）
    }
    if (vx == 0) vx = 1;
    if (vy == 0) vy = 1;
}

void Fish::update()
{
    moveTimer++;
    if (moveTimer % 30 == 0) vx = -vx;
    if (moveTimer % 47 == 0) vy = -vy;
    x += vx;
    y += vy;

    if (x < 50) { x = 50;   vx = abs(vx); }
    if (x > 1230) { x = 1230; vx = -abs(vx); }
    if (y < 60) { y = 60;   vy = abs(vy); }
    if (y > 700) { y = 700;  vy = -abs(vy); }
}

bool Fish::isNearPlayer(int px, int py, int range)
{
    int dx = px - x;
    int dy = py - y;
    return (dx * dx + dy * dy) <= (range * range);
}