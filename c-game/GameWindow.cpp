#include "GameWindow.h"
#include "ShopDialog.h"
#include <QPainter>
#include <QFont>
#include <QMessageBox>
#include <algorithm>

GameWindow::GameWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("渔途");
    setFixedSize(1280, 720);
    gm = new GameManager();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::gameLoop);
    timer->start(16);
}

GameWindow::~GameWindow()
{
    delete gm;
}

void GameWindow::gameLoop()
{
    if (showIntro) return;

    if (gm->gameOver) {
        timer->stop();
        showResult();
        return;
    }

    if (gm->stageClear) {
        timer->stop();
        gm->fileManager.saveGame(
            gm->player->distance, gm->player->coins,
            gm->player->durability, gm->player->stamina);
        openShop();
        gm->stage++;
        gm->bossSpawned = false;
        gm->stageClear = false;
        isFishing = false;
        targetFish = nullptr;
        timer->start(16);
    }

    if (keyUp)    gm->player->move(0, -1);
    if (keyDown)  gm->player->move(0, 1);
    if (keyLeft)  gm->player->move(-1, 0);
    if (keyRight) gm->player->move(1, 0);

    updateFishing();
    gm->update();
    update();
}

void GameWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    if (showIntro) {
        drawIntro(p);
        return;
    }

    drawSea(p);
    drawObstacles(p);
    drawFish(p);
    drawSharks(p);
    drawPlayer(p);
    drawHUD(p);
    drawFishingHUD(p);
}

void GameWindow::drawIntro(QPainter& p)
{
    p.fillRect(0, 0, 1280, 720, QColor(20, 70, 140));

    p.setPen(QPen(QColor(50, 130, 210), 1));
    for (int y = 80; y < 720; y += 60)
        for (int x = 0; x < 1280; x += 80)
            p.drawLine(x, y, x + 40, y);

    p.setPen(Qt::white);
    p.setFont(QFont("Microsoft YaHei", 42, QFont::Bold));
    p.drawText(0, 60, 1280, 100, Qt::AlignCenter, "渔  途");

    p.setFont(QFont("Microsoft YaHei", 14));
    p.setPen(QColor(180, 220, 255));
    p.drawText(0, 150, 1280, 30, Qt::AlignCenter, "—— 一场向右的海上冒险 ——");

    p.setPen(QPen(QColor(100, 160, 220), 1));
    p.drawLine(200, 195, 1080, 195);

    QStringList lines = {
        "【目标】  驾船向右航行，闯过 5 个关卡，击败每关的 Boss 鲨鱼",
        "",
        "【移动】  WASD 或方向键控制船只上下左右移动",
        "",
        "【捕鱼】  靠近鱼群后按 F 键开始捕捉，顶部出现倒计时进度条",
        "             在时间内连续按 F 达到要求次数即可捕获，超时鱼会逃走",
        "             黄色普通鱼：按 3 次，3 秒时间，价值低，体力回复多",
        "             红色稀有鱼：按 8 次，1.5 秒时间，价值高，体力回复少",
        "",
        "【战斗】  按 空格键 攻击附近的鲨鱼",
        "             普通鲨鱼（蓝色）：随时可击杀，击杀获得金币",
        "             Boss 鲨鱼（红色）：血量低于一半时加速并召唤小鲨鱼",
        "",
        "【海浪】  随机触发，船速会临时加快或减慢",
        "",
        "【障碍】  暗礁（棕色方块）：碰到损失耐久度",
        "             漩涡（蓝色圆圈）：碰到减少体力并降低速度",
        "             障碍靠近才会显示，注意提前避开！",
        "",
        "【商店】  击败 Boss 后自动进入岛屿商店，按 P 键也可随时打开",
        "             可购买干粮恢复体力，修理包恢复耐久，鱼叉提升攻击力",
        "",
        "【结算】  耐久归零游戏结束，按距离、金币、击杀数综合评级 S / A / B / C",
    };

    p.setFont(QFont("Microsoft YaHei", 12));
    p.setPen(Qt::white);

    int startY = 210;
    for (const QString& line : lines) {
        if (line.isEmpty()) { startY += 8; continue; }
        p.drawText(180, startY, line);
        startY += 22;
    }

    p.setPen(QPen(QColor(100, 160, 220), 1));
    p.drawLine(200, 678, 1080, 678);

    static int blinkTimer = 0;
    blinkTimer++;
    if ((blinkTimer / 30) % 2 == 0) {
        p.setPen(QColor(255, 220, 80));
        p.setFont(QFont("Microsoft YaHei", 16, QFont::Bold));
        p.drawText(0, 688, 1280, 30, Qt::AlignCenter, "按空格键开始游戏");
    }
}

void GameWindow::drawSea(QPainter& p)
{
    p.fillRect(0, 0, 1280, 720, QColor(30, 100, 180));
    p.setPen(QPen(QColor(50, 130, 210), 1));
    for (int y = 80; y < 720; y += 60)
        for (int x = 0; x < 1280; x += 80)
            p.drawLine(x, y, x + 40, y);
}

void GameWindow::drawFish(QPainter& p)
{
    for (auto f : gm->fish) {
        if (f->caught || f->escaped) continue;

        if (f->type == Fish::EDIBLE)
            p.setBrush(QColor(255, 220, 50));
        else
            p.setBrush(QColor(255, 80, 80));
        p.setPen(Qt::NoPen);
        p.drawEllipse(f->x - 8, f->y - 5, 16, 10);

        // 靠近时显示提示
        if (!isFishing && f->isNearPlayer(gm->player->x, gm->player->y, 80)) {
            p.setPen(Qt::white);
            p.setFont(QFont("Microsoft YaHei", 10));
            p.drawText(f->x - 15, f->y - 14, "按F捕鱼");
        }
    }
}

void GameWindow::drawObstacles(QPainter& p)
{
    for (auto o : gm->obstacles) {
        if (!o->visible) continue;
        if (o->type == REEF) {
            p.setBrush(QColor(120, 80, 40));
            p.setPen(Qt::NoPen);
            p.drawRect(o->x - 15, o->y - 15, 30, 30);
        }
        else {
            p.setBrush(QColor(80, 180, 200, 160));
            p.setPen(QPen(QColor(100, 200, 220), 2));
            p.drawEllipse(o->x - 25, o->y - 25, 50, 50);
        }
    }
}

void GameWindow::drawSharks(QPainter& p)
{
    for (auto s : gm->sharks) {
        if (!s->alive) continue;
        if (s->isBoss)
            p.setBrush(QColor(180, 0, 0));
        else
            p.setBrush(QColor(80, 80, 200));
        p.setPen(Qt::NoPen);
        p.drawEllipse(s->x - 20, s->y - 12, 40, 24);

        p.fillRect(s->x - 20, s->y - 22, 40, 6, QColor(60, 60, 60));
        int barW = (int)(40.0f * s->hp / s->maxHp);
        p.fillRect(s->x - 20, s->y - 22, barW, 6, QColor(220, 50, 50));
    }
}

void GameWindow::drawPlayer(QPainter& p)
{
    p.setBrush(QColor(240, 240, 240));
    p.setPen(QPen(QColor(100, 100, 100), 1));
    p.drawRect(gm->player->x - 20, gm->player->y - 10, 40, 20);
}

void GameWindow::drawHUD(QPainter& p)
{
    p.fillRect(0, 0, 1280, 44, QColor(0, 0, 0, 170));

    p.setFont(QFont("Microsoft YaHei", 10));
    p.setPen(Qt::white);
    p.drawText(10, 14, "耐久");
    p.fillRect(50, 6, 100, 12, QColor(60, 60, 60));
    p.fillRect(50, 6, gm->player->durability, 12, QColor(80, 200, 80));

    p.drawText(165, 14, "体力");
    p.fillRect(205, 6, 100, 12, QColor(60, 60, 60));
    p.fillRect(205, 6, gm->player->stamina, 12, QColor(200, 200, 50));

    p.setFont(QFont("Microsoft YaHei", 11));
    p.drawText(320, 28, QString("金币: %1").arg(gm->player->coins));
    p.drawText(440, 28, QString("距离: %1 m").arg(gm->player->distance));
    p.drawText(580, 28, QString("关卡: %1 / 5").arg(gm->stage));
    p.drawText(680, 28, QString("击杀: %1").arg(gm->killCount));

    if (gm->waves.waveActive) {
        p.setPen(QColor(255, 200, 50));
        p.drawText(800, 28, QString("海浪！速度 x%1")
            .arg((double)gm->waves.speedMultiplier, 0, 'f', 1));
    }

    p.setPen(QColor(180, 180, 180));
    p.setFont(QFont("Microsoft YaHei", 10));
    p.drawText(1020, 28, "空格:攻击  F:捕鱼  P:商店");
}

void GameWindow::drawFishingHUD(QPainter& p)
{
    if (!isFishing || !targetFish) return;

    int barX = 490, barY = 55, barW = 300, barH = 22;

    // 背景
    p.fillRect(barX - 5, barY - 22, barW + 10, barH + 28, QColor(0, 0, 0, 180));

    // 提示文字
    p.setPen(Qt::white);
    p.setFont(QFont("Microsoft YaHei", 11));
    QString fishName = (targetFish->type == Fish::EDIBLE) ? "普通鱼" : "稀有鱼";
    p.drawText(barX, barY - 4, QString("捕捉 %1  —  按F次数: %2 / %3")
        .arg(fishName).arg(fishClickCount).arg(targetFish->catchRequired));

    // 进度条背景
    p.fillRect(barX, barY, barW, barH, QColor(50, 50, 50));

    // 进度条（时间剩余）
    float ratio = 1.0f - (float)fishTimer / targetFish->catchTimeLimit;
    int fillW = (int)(barW * ratio);
    QColor barColor = (targetFish->type == Fish::EDIBLE)
        ? QColor(255, 220, 50) : QColor(255, 80, 80);
    p.fillRect(barX, barY, fillW, barH, barColor);

    // 边框
    p.setPen(QPen(Qt::white, 1));
    p.drawRect(barX, barY, barW, barH);
}

void GameWindow::tryStartFishing()
{
    if (isFishing) return;

    for (auto f : gm->fish) {
        if (f->caught || f->escaped) continue;
        if (f->isNearPlayer(gm->player->x, gm->player->y, 80)) {
            targetFish = f;
            isFishing = true;
            fishClickCount = 0;
            fishTimer = 0;
            return;
        }
    }
}

void GameWindow::updateFishing()
{
    if (!isFishing || !targetFish) return;

    fishTimer++;

    // 超时失败
    if (fishTimer >= targetFish->catchTimeLimit) {
        targetFish->vx *= 3;
        targetFish->vy *= 3;
        targetFish->escaped = true;
        isFishing = false;
        targetFish = nullptr;
        return;
    }

    // 捕捉成功
    if (fishClickCount >= targetFish->catchRequired) {
        targetFish->caught = true;
        gm->player->coins += targetFish->value;
        gm->player->stamina = std::min(100,
            gm->player->stamina + targetFish->staminaGain);
        isFishing = false;
        targetFish = nullptr;
    }
}

void GameWindow::openShop()
{
    ShopDialog dlg(gm->player, this);
    dlg.exec();
}

void GameWindow::showResult()
{
    int score = gm->player->distance / 10
        + gm->player->coins
        + gm->killCount * 50;

    QString grade;
    if (score >= 800) grade = "S";
    else if (score >= 500) grade = "A";
    else if (score >= 300) grade = "B";
    else                   grade = "C";

    gm->fileManager.saveHighScore("Player", score,
        gm->player->distance, gm->killCount);

    QMessageBox::information(this, "游戏结束",
        QString("评级：%1\n得分：%2\n航行距离：%3 m\n击杀数：%4\n金币：%5")
        .arg(grade).arg(score)
        .arg(gm->player->distance)
        .arg(gm->killCount)
        .arg(gm->player->coins));

    close();
}

void GameWindow::keyPressEvent(QKeyEvent* event)
{
    if (showIntro) {
        if (event->key() == Qt::Key_Space) {
            showIntro = false;
            update();
        }
        return;
    }

    switch (event->key()) {
    case Qt::Key_W: case Qt::Key_Up:    keyUp = true; break;
    case Qt::Key_S: case Qt::Key_Down:  keyDown = true; break;
    case Qt::Key_A: case Qt::Key_Left:  keyLeft = true; break;
    case Qt::Key_D: case Qt::Key_Right: keyRight = true; break;
    case Qt::Key_F:
        if (!isFishing)
            tryStartFishing();
        else
            fishClickCount++;
        break;
    case Qt::Key_Space:
        gm->attackNearestShark(30, 150);
        break;
    case Qt::Key_P:
        timer->stop();
        openShop();
        timer->start(16);
        break;
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (showIntro) return;

    switch (event->key()) {
    case Qt::Key_W: case Qt::Key_Up:    keyUp = false; break;
    case Qt::Key_S: case Qt::Key_Down:  keyDown = false; break;
    case Qt::Key_A: case Qt::Key_Left:  keyLeft = false; break;
    case Qt::Key_D: case Qt::Key_Right: keyRight = false; break;
    }
}