#include "Item.h"

Item::Item(QString name, int price)
    : name(name), price(price) {
}

FoodItem::FoodItem() : Item("¸ÉÁ¸", 20) {}
void FoodItem::use(Player& player)
{
    player.stamina += 30;
    if (player.stamina > 100) player.stamina = 100;
}

RepairKit::RepairKit() : Item("ĞŞÀí°ü", 50) {}
void RepairKit::use(Player& player)
{
    player.durability += 40;
    if (player.durability > 100) player.durability = 100;
}