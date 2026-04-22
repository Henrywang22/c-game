#include "Fish.h"
#include "Player.h"
#include <cstdlib>
#include <cmath>

Fish::Fish(int x, int y, Type type)
    : x(x), y(y), type(type), maxLife(300 + rand() % 120) {
}

void Fish::changeDirection()
{
    float angle = (rand() % 360) * 3.14159f / 180.0f;
    float speed = sqrt(vx * vx + vy * vy);
    if (speed < 0.5f) speed = 1.0f;
    vx = speed * cos(angle);
    vy = speed * sin(angle);
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
    float angle = (rand() % 360) * 3.14159f / 180.0f;
    float speed = 1.0f + (rand() % 10) * 0.1f;
    vx = speed * cos(angle);
    vy = speed * sin(angle);
    catchRequired = 3;
    catchTimeLimit = 180;
}

void Fish::update(int playerX, int playerY)
{
    lifeTimer++;
    moveTimer++;
    if (lifeTimer >= maxLife) { escaped = true; return; }
    x += (int)vx;
    y += (int)vy;
    if (x < 0 || x > 2560) { escaped = true; return; }
    if (y < 60) { y = 60;  vy = -vy; }
    if (y > 700) { y = 700; vy = -vy; }
}

void CommonFish::update(int playerX, int playerY)
{
    lifeTimer++;
    moveTimer++;

    if (lifeTimer >= maxLife) { escaped = true; return; }

    if (fleeCooldown > 0) fleeCooldown--;

    // 感知玩家后逃跑
    int dx = x - playerX;
    int dy = y - playerY;
    float dist = sqrt((float)(dx * dx + dy * dy));
    if (dist < 120 && fleeCooldown <= 0 && !fleeing) {
        fleeing = true;
        fleeCooldown = 180;
        float len = dist > 0 ? dist : 1;
        vx = (dx / len) * 2.5f;
        vy = (dy / len) * 2.5f;
    }
    if (fleeing && fleeCooldown <= 120) fleeing = false;

    if (moveTimer % 90 == 0 && !fleeing) changeDirection();

    x += (int)vx;
    y += (int)vy;

    // 边界处理
    if (x < 0 || x > 2560) { escaped = true; return; }
    if (y < 60) { y = 60;  vy = abs(vy); }
    if (y > 700) { y = 700; vy = -abs(vy); }
}

// RareFish
RareFish::RareFish(int x, int y, Type type) : Fish(x, y, type)
{
    float angle = (rand() % 360) * 3.14159f / 180.0f;
    float speed = 2.0f + (rand() % 15) * 0.1f;
    vx = speed * cos(angle);
    vy = speed * sin(angle);
    catchRequired = 8;
    catchTimeLimit = 90;
}

void RareFish::update(int playerX, int playerY)
{
    lifeTimer++;
    moveTimer++;

    if (lifeTimer >= maxLife) { escaped = true; return; }

    if (fleeCooldown > 0) fleeCooldown--;

    int dx = x - playerX;
    int dy = y - playerY;
    float dist = sqrt((float)(dx * dx + dy * dy));
    if (dist < 150 && fleeCooldown <= 0) {
        fleeing = true;
        fleeCooldown = 120;
        float len = dist > 0 ? dist : 1;
        vx = (dx / len) * 4.0f;
        vy = (dy / len) * 4.0f;
    }
    if (fleeing && fleeCooldown <= 60) fleeing = false;

    if (moveTimer % 60 == 0 && !fleeing) changeDirection();

    x += (int)vx;
    y += (int)vy;

    if (x < 0 || x > 2560) { escaped = true; return; }
    if (y < 60) { y = 60;  vy = abs(vy); }
    if (y > 700) { y = 700; vy = -abs(vy); }
}

// Sardine
Sardine::Sardine(int x, int y) : CommonFish(x, y, SARDINE)
{
    value = 5 + rand() % 10;
    staminaGain = 10;
}

// Tuna
Tuna::Tuna(int x, int y) : CommonFish(x, y, TUNA)
{
    value = 25 + rand() % 20;
    staminaGain = 20;
}

// DeepSeaEel
DeepSeaEel::DeepSeaEel(int x, int y) : RareFish(x, y, DEEPSEAEEL)
{
    value = 80 + rand() % 60;
    staminaGain = 5;
}

// GoldenFish
GoldenFish::GoldenFish(int x, int y) : RareFish(x, y, SWORDFISH_FISH)
{
    value = 150 + rand() % 100;
    staminaGain = 30;
    catchRequired = 10;
    catchTimeLimit = 75;
}