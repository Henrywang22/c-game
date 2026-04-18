#include "Weapon.h"

// Harpoon
Harpoon::Harpoon() : Weapon(HARPOON)
{
    name = "鱼叉";
    damage = 30;
    durability = 20;
    range = 100;
}
int Harpoon::fire()
{
    if (durability <= 0) return 0;
    durability--;
    return damage;
}

// Cannon
Cannon::Cannon() : Weapon(CANNON)
{
    name = "炮台";
    damage = 60;
    durability = 10;
    range = 200;
}
int Cannon::fire()
{
    if (durability <= 0) return 0;
    durability--;
    return damage;
}

// Net
Net::Net() : Weapon(NET)
{
    name = "渔网";
    damage = 10;
    durability = 30;
    range = 80;
}
int Net::fire()
{
    if (durability <= 0) return 0;
    durability--;
    return damage;
}