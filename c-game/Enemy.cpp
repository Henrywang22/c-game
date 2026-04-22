#include "Enemy.h"

Enemy::Enemy(int x, int y)
    : x(x), y(y), hp(100), maxHp(100),
    attack(10), speed(2.0f), dropValue(50) {
}