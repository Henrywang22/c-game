#pragma once
#include <QString>
#include "Player.h"

class Item {
public:
    Item(QString name, int price);
    virtual void use(Player& player) = 0;
    virtual ~Item() {}

    QString name;
    int price;
};

class FoodItem : public Item {
public:
    FoodItem();
    void use(Player& player) override;
};

class RepairKit : public Item {
public:
    RepairKit();
    void use(Player& player) override;
};
