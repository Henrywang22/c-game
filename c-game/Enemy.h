#pragma once
#include "Player.h"

class Enemy {
public:
    Enemy(int x, int y);
    virtual ~Enemy() {}
    virtual void update(Player& player) = 0;
    virtual bool collidesWithPlayer(int px, int py) = 0;

    int x, y;
    int hp;
    int maxHp;
    int attack;
    float speed;
    bool alive = true;
    int attackTimer = 0;
    int dropValue;
};
