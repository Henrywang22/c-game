#pragma once
#include "Player.h"

class Shark {
public:
    enum State { HUNT, ENRAGED };

    Shark(int x, int y, bool isBoss = false);
    void update(Player& player);
    bool collidesWithPlayer(int px, int py);

    int x, y;
    int hp;
    int maxHp;
    int attack;
    float speed;
    State state = HUNT;
    bool isBoss;
    bool alive = true;
    int attackTimer = 0;
    int dropValue;
};
