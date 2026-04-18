#include "Obstacle.h"
#include "Player.h"

Obstacle::Obstacle(int x, int y, ObstacleType type)
    : x(x), y(y), type(type) {
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

Whirlpool::Whirlpool(int x, int y) : Obstacle(x, y, WHIRLPOOL) {}

void Whirlpool::triggerEffect(Player& player)
{
    player.stamina -= 5;
    player.speed = 1;
    if (player.stamina < 0) player.stamina = 0;
}