#include "Boss.h"
#include "NormalEnemy.h"
#include <cmath>

Boss::Boss(int x, int y) : Enemy(x, y)
{
    hp = maxHp = 500;
    attack = 20;
    speed = 2.5f;
    dropValue = 200;
}

void Boss::update(Player& player)
{
    if (!alive) return;
    phaseTimer++;

    if (hp < maxHp / 2 && state == PHASE1) {
        state = PHASE2;
        speed = 4.0f;
    }

    float dx = (float)(player.x - x);
    float dy = (float)(player.y - y);
    float dist = sqrt(dx * dx + dy * dy);
    if (dist > 0) {
        x += (int)(speed * dx / dist);
        y += (int)(speed * dy / dist);
    }
}

bool Boss::collidesWithPlayer(int px, int py)
{
    int dx = px - x;
    int dy = py - y;
    return (dx * dx + dy * dy) <= (40 * 40);
}

void Boss::spawnMinions(std::vector<Shark*>& sharks)
{
    if (minionSpawned) return;
    minionSpawned = true;
    sharks.push_back(new Shark(x + 80, y - 60));
    sharks.push_back(new Shark(x + 80, y + 60));
}