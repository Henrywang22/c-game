#pragma once
#include <QString>

class Weapon {
public:
    enum WeaponType { HARPOON, CANNON, NET };

    QString name;
    int damage = 0;
    int durability = 0;
    int range = 0;
    WeaponType type;

    Weapon(WeaponType t) : type(t) {}
    virtual int fire() = 0;
    virtual ~Weapon() {}
};

class Harpoon : public Weapon {
public:
    Harpoon();
    int fire() override;
};

class Cannon : public Weapon {
public:
    Cannon();
    int fire() override;
};

class Net : public Weapon {
public:
    Net();
    int fire() override;
};