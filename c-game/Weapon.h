#pragma once
#include <QString>

class Weapon {
public:
    enum WeaponType { FISHING_ROD, NET, HARPOON, PISTOL, SHOTGUN };

    QString name;
    int damage;
    int durability;
    int maxDurability;
    int range;
    int upgradeLevel = 0;
    WeaponType type;

    Weapon(WeaponType t) : type(t), damage(0), durability(0), maxDurability(0), range(0) {}
    virtual int fire() = 0;
    void upgrade();
    virtual ~Weapon() {}
};

class FishingRod : public Weapon {
public:
    FishingRod();
    int fire() override;
};

class FishNet : public Weapon {
public:
    FishNet();
    int fire() override;
};

class Harpoon : public Weapon {
public:
    Harpoon();
    int fire() override;
};

class Pistol : public Weapon {
public:
    Pistol();
    int fire() override;
};

class Shotgun : public Weapon {
public:
    Shotgun();
    int fire() override;
};