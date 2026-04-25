#include "Weapon.h"

void Weapon::upgrade()
{
    upgradeLevel++;
    maxDurability += 10;
    durability = maxDurability;
    damage += 5;
}

FishingRod::FishingRod() : Weapon(FISHING_ROD)
{
    name = "鱼竿"; damage = 5; maxDurability = 100;
    durability = 100; range = 60;
}
int FishingRod::fire()
{
    if (durability <= 0) return 0;
    durability--;
    return damage;
}
FishNet::FishNet() : Weapon(NET)
{
    name = "渔网"; damage = 8; maxDurability = 80;
    durability = 80; range = 80;
}
int FishNet::fire()
{
    if (durability <= 0) return 0;
    durability--;
    return damage;
}

Harpoon::Harpoon() : Weapon(HARPOON)
{
    name = "鱼叉"; damage = 30; maxDurability = 25;
    durability = 60; range = 120;
}
int Harpoon::fire()
{
    if (durability <= 0) return 0;
    durability--;
    return damage;
}

Pistol::Pistol() : Weapon(PISTOL)
{
    name = "手枪"; damage = 50; maxDurability = 30;
    durability = 30; range = 200;
}
int Pistol::fire()
{
    if (durability <= 0) return 0;
    durability--;
    return damage;
}
Shotgun::Shotgun() : Weapon(SHOTGUN)
{
    name = "猎枪"; damage = 80; maxDurability = 20;
    durability = 20; range = 150;
}
int Shotgun::fire()
{
    if (durability <= 0) return 0;
    durability--;
    return damage;
}