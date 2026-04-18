#include "Player.h"

Player::Player(int startX, int startY)
    : x(startX), y(startY),
    durability(100),
    stamina(100),
    coins(0),
    distance(0)
{
}

void Player::move(int dx, int dy)
{
    x += dx * speed;
    y += dy * speed;

    // 边界限制，不让船跑出屏幕
    if (x < 20)   x = 20;
    if (x > 1260) x = 1260;
    if (y < 50)   y = 50;   // HUD 下方
    if (y > 710)  y = 710;

    // 向右移动增加距离
    if (dx > 0) distance += speed;
}