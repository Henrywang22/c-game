#include "ShopDialog.h"
#include "Item.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

ShopDialog::ShopDialog(Player* player, QWidget* parent)
    : QDialog(parent), player(player)
{
    setWindowTitle("岛屿商店");
    setFixedSize(300, 320);

    QVBoxLayout* layout = new QVBoxLayout(this);

    coinLabel = new QLabel(this);
    updateCoinLabel();
    layout->addWidget(coinLabel);

    auto makeBtn = [&](QString text, auto slot) {
        QPushButton* btn = new QPushButton(text, this);
        connect(btn, &QPushButton::clicked, this, slot);
        layout->addWidget(btn);
        };

    makeBtn("购买干粮 (20金币) — 恢复30体力", &ShopDialog::buyFood);
    makeBtn("购买修理包 (50金币) — 恢复40耐久", &ShopDialog::buyRepairKit);
    makeBtn("购买鱼叉 (80金币)", &ShopDialog::buyHarpoon);

    QPushButton* closeBtn = new QPushButton("离开商店", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeBtn);
}

void ShopDialog::updateCoinLabel()
{
    coinLabel->setText(QString("当前金币：%1").arg(player->coins));
}

void ShopDialog::buyFood()
{
    FoodItem food;
    if (player->coins < food.price) {
        QMessageBox::warning(this, "提示", "金币不足！");
        return;
    }
    player->coins -= food.price;
    food.use(*player);
    updateCoinLabel();
    QMessageBox::information(this, "购买成功", "体力已恢复30点");
}

void ShopDialog::buyRepairKit()
{
    RepairKit kit;
    if (player->coins < kit.price) {
        QMessageBox::warning(this, "提示", "金币不足！");
        return;
    }
    player->coins -= kit.price;
    kit.use(*player);
    updateCoinLabel();
    QMessageBox::information(this, "购买成功", "耐久已恢复40点");
}

void ShopDialog::buyHarpoon()
{
    if (player->coins < 80) {
        QMessageBox::warning(this, "提示", "金币不足！");
        return;
    }
    player->coins -= 80;
    updateCoinLabel();
    QMessageBox::information(this, "购买成功", "获得鱼叉！");
}