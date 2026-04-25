#include "Obstacle.h"
#include "Player.h"
#include <cstdlib>
#include <cmath>

Obstacle::Obstacle(int x, int y, ObstacleType type)
    : x(x), y(y), type(type)
{
    size = 20 + rand() % 20;
    revealRange = 250;  // 改这里
}

bool Obstacle::isVisible(int px, int py)
{
    int dx = px - x;
    int dy = py - y;
    visible = (dx * dx + dy * dy) <= (revealRange * revealRange);
    return visible;
}

Reef::Reef(int x, int y) : Obstacle(x, y, REEF) {}

void Reef::triggerEffect(Player& player)
{
    player.durability -= 10;
    if (player.durability < 0) player.durability = 0;
}

void Reef::applyRebound(Player& player, float collisionSpeed)
{
    int rebound = (int)(collisionSpeed * 20);
    // 推开玩家
    if (player.x < x) player.x -= rebound;
    else player.x += rebound;
    if (player.y < y) player.y -= rebound;
    else player.y += rebound;
}

Whirlpool::Whirlpool(int x, int y) : Obstacle(x, y, WHIRLPOOL) {}

void Whirlpool::triggerEffect(Player& player)
{
    player.stamina -= 3;
    if (player.stamina < 0) player.stamina = 0;
    // 按比例减速，最多减为0
    player.speed = (int)(player.speed * 0.7f);
    if (player.speed < 0) player.speed = 0;
}