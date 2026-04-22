#include "Player.h"
#include <algorithm>

Player::Player(int startX, int startY)
    : x(startX), y(startY),
    durability(100), maxDurability(100),
    stamina(100), maxStamina(100),
    coins(0), distance(0),
    speed(3), baseSpeed(3)
{
}

void Player::move(int dx, int dy, int cameraX)
{
    int currentSpeed = boosting ? std::min(speed * 2, 8) : speed;

    x += dx * currentSpeed;
    y += dy * currentSpeed;

    if (boosting) {
        stamina -= 1;
        if (stamina < 0) stamina = 0;
        if (stamina == 0) boosting = false;
    }

    // ×ó±ß½ç£ºËæ¾íÖáÒÆ¶¯£¬ÎÞ·¨Íù×ó³¬¹ýÆÁÄ»×ó±ß
    int leftLimit = cameraX + 20;
    if (x < leftLimit) x = leftLimit;
    if (x > 5000) x = 5000;
    if (y < 50)   y = 50;
    if (y > 710)  y = 710;

    if (dx > 0) distance += currentSpeed;
}

void Player::boost()
{
    if (stamina > 10) boosting = true;
}

void Player::stopBoost()
{
    boosting = false;
}