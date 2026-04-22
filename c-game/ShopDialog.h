#pragma once
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "Player.h"

class ShopDialog : public QDialog {
    Q_OBJECT
public:
    ShopDialog(Player* player, QWidget* parent = nullptr);

private slots:
    void buyFood();
    void buyRepairKit();
    void buyHarpoon();
    void buyPistol();
    void upgradeStamina();
    void upgradeDurability();
    void upgradeSpeed();

private:
    Player* player;
    QLabel* coinLabel;
    QLabel* statusLabel;
    void updateLabels();
    bool spend(int cost);
};