#include "ShopDialog.h"
#include "Item.h"
#include "Weapon.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

ShopDialog::ShopDialog(Player* player, QWidget* parent)
    : QDialog(parent), player(player)
{
    setWindowTitle("岛屿商店");
    setFixedSize(360, 480);

    QVBoxLayout* layout = new QVBoxLayout(this);

    coinLabel = new QLabel(this);
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: green;");
    layout->addWidget(coinLabel);
    layout->addWidget(statusLabel);

    auto addBtn = [&](QString text, auto slot) {
        QPushButton* btn = new QPushButton(text, this);
        connect(btn, &QPushButton::clicked, this, slot);
        layout->addWidget(btn);
        };

    addBtn("购买干粮 (20金币) — 恢复30体力", &ShopDialog::buyFood);
    addBtn("购买修理包 (50金币) — 恢复40耐久", &ShopDialog::buyRepairKit);
    addBtn("购买鱼叉 (80金币)", &ShopDialog::buyHarpoon);
    addBtn("购买手枪 (150金币)", &ShopDialog::buyPistol);
    addBtn("体力上限升级 (100金币) +20体力上限", &ShopDialog::upgradeStamina);
    addBtn("耐久上限升级 (100金币) +20耐久上限", &ShopDialog::upgradeDurability);
    addBtn("船速升级 (120金币) +1基础速度", &ShopDialog::upgradeSpeed);

    QPushButton* closeBtn = new QPushButton("离开商店", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeBtn);

    updateLabels();
}

void ShopDialog::updateLabels()
{
    coinLabel->setText(QString("金币：%1    耐久：%2/%3    体力：%4/%5")
        .arg(player->coins)
        .arg(player->durability).arg(player->maxDurability)
        .arg(player->stamina).arg(player->maxStamina));
}

bool ShopDialog::spend(int cost)
{
    if (player->coins < cost) {
        statusLabel->setText("金币不足！");
        return false;
    }
    player->coins -= cost;
    updateLabels();
    return true;
}

void ShopDialog::buyFood()
{
    if (!spend(20)) return;
    player->stamina = std::min(player->maxStamina, player->stamina + 30);
    statusLabel->setText("购买成功！体力 +30");
    updateLabels();
}

void ShopDialog::buyRepairKit()
{
    if (!spend(50)) return;
    player->durability = std::min(player->maxDurability, player->durability + 40);
    statusLabel->setText("购买成功！耐久 +40");
    updateLabels();
}

void ShopDialog::buyHarpoon()
{
    if (!spend(80)) return;
    statusLabel->setText("购买成功！获得鱼叉");
}

void ShopDialog::buyPistol()
{
    if (!spend(150)) return;
    statusLabel->setText("购买成功！获得手枪");
}

void ShopDialog::upgradeStamina()
{
    if (!spend(100)) return;
    player->maxStamina += 20;
    player->stamina = std::min(player->stamina + 20, player->maxStamina);
    statusLabel->setText("升级成功！体力上限 +20");
    updateLabels();
}

void ShopDialog::upgradeDurability()
{
    if (!spend(100)) return;
    player->maxDurability += 20;
    player->durability = std::min(player->durability + 20, player->maxDurability);
    statusLabel->setText("升级成功！耐久上限 +20");
    updateLabels();
}

void ShopDialog::upgradeSpeed()
{
    if (!spend(120)) return;
    player->baseSpeed += 1;
    statusLabel->setText("升级成功！基础速度 +1");
    updateLabels();
}