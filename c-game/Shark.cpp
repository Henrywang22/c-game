#include "Shark.h"
#include <cmath>
#include <cstdlib>

Shark::Shark(int x, int y, bool isBoss)
    : x(x), y(y), isBoss(isBoss)
{
    if (isBoss) {
        hp = maxHp = 500;
        attack = 20;
        speed = 2.5f;
        dropValue = 200;
    }
    else {
        hp = maxHp = 150;
        attack = 10;
        speed = 3.5f;
        dropValue = 50;
    }
}

void Shark::update(Player& player)
{
    if (!alive) return;

    if (hp < maxHp / 2) {
        state = ENRAGED;
        speed = isBoss ? 4.0f : 5.0f;
    }

    float dx = player.x - x;
    float dy = player.y - y;
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