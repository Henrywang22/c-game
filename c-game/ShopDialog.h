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

private:
    Player* player;
    QLabel* coinLabel;
    void updateCoinLabel();
};
