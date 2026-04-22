#pragma once
#include "Enemy.h"

// ∆’Õ®ˆË”„
class Shark : public Enemy {
public:
    enum State { HUNT, ENRAGED };
    Shark(int x, int y);
    void update(Player& player) override;
    bool collidesWithPlayer(int px, int py) override;
    State state = HUNT;
};

// Ω£”„
class Swordfish : public Enemy {
public:
    enum State { IDLE, WINDUP, CHARGE };
    Swordfish(int x, int y);
    void update(Player& player) override;
    bool collidesWithPlayer(int px, int py) override;

    State state = IDLE;
    int alertRange = 250;
    int windupTimer = 0;
    int windupDuration = 90;
    float chargeVx = 0;
    float chargeVy = 0;
    float chargeSpeed = 8.0f;
    int chargeTimer = 0;
    int chargeDuration = 60;
};

// ƒ´”„
class Octopus : public Enemy {
public:
    Octopus(int x, int y);
    void update(Player& player) override;
    bool collidesWithPlayer(int px, int py) override;

    bool visible = true;
    bool isInvisible = false;
    int visTimer = 0;
    int invisTimer = 0;
    int visDuration = 300;
    int invisDuration = 30;
    int contactTimer = 0;
    float vx = 0, vy = 0;
};