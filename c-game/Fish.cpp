#include "Fish.h"
#include <cstdlib>
#include <cmath>

Fish::Fish(int x, int y, Type type)
    : x(x), y(y), type(type) {
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

// CommonFish
CommonFish::CommonFish(int x, int y, Type type) : Fish(x, y, type)
{
    vx = (rand() % 3) - 1;
    vy = (rand() % 3) - 1;
    if (vx == 0) vx = 1;
    if (vy == 0) vy = 1;
    catchRequired = 3;
    catchTimeLimit = 180;
}

// RareFish
RareFish::RareFish(int x, int y, Type type) : Fish(x, y, type)
{
    vx = (rand() % 5) - 2;
    vy = (rand() % 5) - 2;
    if (vx == 0) vx = 2;
    if (vy == 0) vy = 2;
    catchRequired = 8;
    catchTimeLimit = 90;
}

// Sardine — 沙丁鱼，价值低，数量多
Sardine::Sardine(int x, int y) : CommonFish(x, y, SARDINE)
{
    value = 5 + rand() % 10;
    staminaGain = 15;
}
int Sardine::getEconomicValue() { return value; }

// Tuna — 金枪鱼，价值中等
Tuna::Tuna(int x, int y) : CommonFish(x, y, TUNA)
{
    value = 20 + rand() % 30;
    staminaGain = 25;
}
int Tuna::getEconomicValue() { return value; }

// DeepSeaEel — 深海鳗，价值高，难捕捉
DeepSeaEel::DeepSeaEel(int x, int y) : RareFish(x, y, DEEPSEAEEL)
{
    value = 80 + rand() % 100;
    staminaGain = 8;
}
int DeepSeaEel::getEconomicValue() { return value; }