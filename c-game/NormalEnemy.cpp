#include "NormalEnemy.h"
#include <cmath>
#include <cstdlib>

// ©¤©¤©¤ Shark ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤

Shark::Shark(int x, int y) : Enemy(x, y)
{
    hp = maxHp = 150;
    attack = 10;
    speed = 3.5f;
    dropValue = 50;
}

void Shark::update(Player& player)
{
    if (!alive) return;

    if (hp < maxHp / 2 && state == HUNT) {
        state = ENRAGED;
        speed = 5.0f;
    }

    float dx = (float)(player.x - x);
    float dy = (float)(player.y - y);
    float dist = sqrt(dx * dx + dy * dy);
    if (dist > 0) {
        x += (int)(speed * dx / dist);
        y += (int)(speed * dy / dist);
    }
}

bool Shark::collidesWithPlayer(int px, int py)
{
    int dx = px - x;
    int dy = py - y;
    return (dx * dx + dy * dy) <= (30 * 30);
}

// ©¤©¤©¤ Swordfish ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤

Swordfish::Swordfish(int x, int y) : Enemy(x, y)
{
    hp = maxHp = 100;
    attack = 25;
    speed = 0;
    dropValue = 80;
}

void Swordfish::update(Player& player)
{
    if (!alive) return;

    float dx = (float)(player.x - x);
    float dy = (float)(player.y - y);
    float dist = sqrt(dx * dx + dy * dy);

    switch (state) {
    case IDLE:
        if (dist < alertRange) {
            state = WINDUP;
            windupTimer = 0;
            if (dist > 0) {
                chargeVx = (dx / dist) * chargeSpeed;
                chargeVy = (dy / dist) * chargeSpeed;
            }
        }
        break;

    case WINDUP:
        windupTimer++;
        if (windupTimer % 10 == 0 && dist > 0) {
            chargeVx = (dx / dist) * chargeSpeed;
            chargeVy = (dy / dist) * chargeSpeed;
        }
        if (windupTimer >= windupDuration) {
            state = CHARGE;
            chargeTimer = 0;
        }
        break;

    case CHARGE:
        x += (int)chargeVx;
        y += (int)chargeVy;
        chargeTimer++;
        if (y < 60) { y = 60;  chargeVy = abs(chargeVy); }
        if (y > 700) { y = 700; chargeVy = -abs(chargeVy); }
        if (chargeTimer >= chargeDuration) {
            state = IDLE;
            windupTimer = 0;
            chargeTimer = 0;
        }
        break;
    }
}

bool Swordfish::collidesWithPlayer(int px, int py)
{
    int dx = px - x;
    int dy = py - y;
    return (dx * dx + dy * dy) <= (25 * 25);
}

// ©¤©¤©¤ Octopus ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤

Octopus::Octopus(int x, int y) : Enemy(x, y)
{
    hp = maxHp = 80;
    attack = 5;
    speed = 0.3f;
    dropValue = 30;

    float angle = (rand() % 360) * 3.14159f / 180.0f;
    vx = speed * cos(angle);
    vy = speed * sin(angle);
}

void Octopus::update(Player& player)
{
    if (!alive) return;

    x += (int)(vx * 10) / 10;
    y += (int)(vy * 10) / 10;

    if (x < 50) { x = 50;   vx = abs(vx); }
    if (x > 2560) { x = 2560; vx = -abs(vx); }
    if (y < 60) { y = 60;   vy = abs(vy); }
    if (y > 700) { y = 700;  vy = -abs(vy); }

    if (!isInvisible) {
        visTimer++;
        if (visTimer >= visDuration) {
            isInvisible = true;
            visible = false;
            visTimer = 0;
        }
    }
    else {
        invisTimer++;
        if (invisTimer >= invisDuration) {
            isInvisible = false;
            visible = true;
            invisTimer = 0;
        }
    }

    if (collidesWithPlayer(player.x, player.y) && !isInvisible)
        contactTimer++;
    else
        if (contactTimer > 0) contactTimer--;
}

bool Octopus::collidesWithPlayer(int px, int py)
{
    int dx = px - x;
    int dy = py - y;
    return (dx * dx + dy * dy) <= (30 * 30);
}