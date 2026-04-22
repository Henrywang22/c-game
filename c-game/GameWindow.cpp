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

GameWindow::~GameWindow() { delete gm; }

void GameWindow::gameLoop()
{
    switch (state) {
    case STATE_PLAYING:
        if (gm->gameOver) { state = STATE_DEFEAT; update(); return; }
        if (gm->victory) { state = STATE_VICTORY; update(); return; }

        if (gm->stageClear) {
            timer->stop();
            gm->fileManager.saveGame({ gm->stage, gm->player->distance,
                gm->player->coins, gm->player->durability, gm->player->stamina,
                gm->player->fishCaught, gm->player->fishTotalValue,
                gm->player->gameSeconds, false });
            openShop();
            // 清理障碍和鲨鱼准备下一关
            for (auto o : gm->obstacles) delete o;
            gm->obstacles.clear();
            for (auto s : gm->sharks) delete s;
            gm->sharks.clear();
            gm->stage++;
            gm->bossSpawned = false;
            gm->stageClear = false;
            isFishing = false;
            targetFish = nullptr;
            gm->spawnObstacles();
            timer->start(16);
            return;
        }

        if (keyUp)    gm->player->move(0, -1, gm->cameraX);
        if (keyDown)  gm->player->move(0, 1, gm->cameraX);
        if (keyLeft)  gm->player->move(-1, 0, gm->cameraX);
        if (keyRight) gm->player->move(1, 0, gm->cameraX);

        if (keyShift) gm->player->boost();
        else          gm->player->stopBoost();

        updateFishing();
        gm->update();
        break;
    default: break;
    }
    update();
}

void GameWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    switch (state) {
    case STATE_INTRO:   drawIntro(p);   break;
    case STATE_MENU:    drawMenu(p);    break;
    case STATE_PLAYING:
    case STATE_PAUSED:  drawGame(p);    break;
    case STATE_DEFEAT:  drawDefeat(p);  break;
    case STATE_VICTORY: drawVictory(p); break;
    default: break;
    }
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
        "【移动】  WASD 移动    Shift 加速（消耗体力）",
        "",
        "【捕鱼】  靠近鱼后按 F 开始捕捉，在倒计时内连续按 F 完成捕获",
        "             黄色沙丁鱼：价值低，易捕    蓝色金枪鱼：价值中，易捕",
        "             紫色深海鳗：价值高，难捕    金色金鱼：价值极高，极难捕",
        "",
        "【战斗】  空格键攻击附近鲨鱼（消耗武器耐久）",
        "             普通鲨鱼（蓝色）    Boss 鲨鱼（红色，血量低于一半时进入狂暴）",
        "",
        "【障碍】  暗礁：碰撞损失耐久并反弹    漩涡：减少体力并降速",
        "             海浪：顺风加速，逆风减速（提前可见）",
        "",
        "【商店】  击败 Boss 后进入，按 P 随时打开    ESC 暂停",
        "",
        "【存档】  按 Q 保存并退出，下次可继续上一关",
    };

    p.setFont(QFont("Microsoft YaHei", 11));
    p.setPen(Qt::white);
    int startY = 210;
    for (const QString& line : lines) {
        if (line.isEmpty()) { startY += 6; continue; }
        p.drawText(160, startY, line);
        startY += 21;
    }

    p.setPen(QPen(QColor(100, 160, 220), 1));
    p.drawLine(200, 678, 1080, 678);

    static int blink = 0; blink++;
    if ((blink / 30) % 2 == 0) {
        p.setPen(QColor(255, 220, 80));
        p.setFont(QFont("Microsoft YaHei", 16, QFont::Bold));
        p.drawText(0, 688, 1280, 30, Qt::AlignCenter, "按空格键开始游戏");
    }
}

void GameWindow::drawMenu(QPainter& p)
{
    p.fillRect(0, 0, 1280, 720, QColor(20, 70, 140));
    p.setPen(QPen(QColor(50, 130, 210), 1));
    for (int y = 80; y < 720; y += 60)
        for (int x = 0; x < 1280; x += 80)
            p.drawLine(x, y, x + 40, y);

    p.setPen(Qt::white);
    p.setFont(QFont("Microsoft YaHei", 42, QFont::Bold));
    p.drawText(0, 180, 1280, 100, Qt::AlignCenter, "渔  途");

    p.setFont(QFont("Microsoft YaHei", 20));
    p.setPen(QColor(255, 220, 80));
    p.drawText(0, 350, 1280, 50, Qt::AlignCenter, "按 N 新开游戏");

    if (gm->fileManager.hasSave()) {
        p.setPen(QColor(100, 220, 255));
        p.drawText(0, 420, 1280, 50, Qt::AlignCenter, "按 C 继续上一关");
    }
}

void GameWindow::drawGame(QPainter& p)
{
    drawSea(p);
    drawObstacles(p);
    drawWaves(p);
    drawFish(p);
    drawSharks(p);
    drawPlayer(p);
    drawHUD(p);
    drawFishingHUD(p);
    gm->weather.draw(p, gm->cameraX);  

    if (state == STATE_PAUSED) drawPaused(p);
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
        int screenX = f->x - gm->cameraX;
        if (screenX < -20 || screenX > 1300) continue;

        switch (f->type) {
        case Fish::SARDINE:      p.setBrush(QColor(255, 220, 50));  break;
        case Fish::TUNA:         p.setBrush(QColor(50, 180, 255));  break;
        case Fish::DEEPSEAEEL:   p.setBrush(QColor(180, 50, 255));  break;
        case Fish::SWORDFISH_FISH: p.setBrush(QColor(255, 180, 0)); break;
        }
        p.setPen(Qt::NoPen);
        p.drawEllipse(screenX - 8, f->y - 5, 16, 10);

        if (!isFishing && f->isNearPlayer(gm->player->x, gm->player->y, 80)) {
            p.setPen(Qt::white);
            p.setFont(QFont("Microsoft YaHei", 10));
            p.drawText(screenX - 15, f->y - 14, "按F捕鱼");
        }
    }
}

void GameWindow::drawObstacles(QPainter& p)
{
    for (auto o : gm->obstacles) {
        if (!o->visible) continue;
        int screenX = o->x - gm->cameraX;
        if (screenX < -50 || screenX > 1330) continue;

        if (o->type == REEF) {
            p.setBrush(QColor(120, 80, 40));
            p.setPen(Qt::NoPen);
            p.drawRect(screenX - o->size, o->y - o->size, o->size * 2, o->size * 2);
        }
        else {
            p.setBrush(QColor(80, 180, 200, 160));
            p.setPen(QPen(QColor(100, 200, 220), 2));
            p.drawEllipse(screenX - o->size, o->y - o->size, o->size * 2, o->size * 2);
        }
    }
}

void GameWindow::drawWaves(QPainter& p)
{
    for (auto& w : gm->waves.waves) {
        if (!w.active) continue;
        int screenX = w.x - gm->cameraX;
        if (screenX < -100 || screenX > 1380) continue;

        p.setPen(QPen(QColor(150, 200, 255, 180), 3));
        p.drawLine(screenX - 40, w.y, screenX + 40, w.y);
        p.drawLine(screenX - 20, w.y - 8, screenX + 20, w.y - 8);

        // 方向箭头
        p.setPen(QPen(Qt::white, 2));
        if (w.dir == WAVE_RIGHT)
            p.drawText(screenX + 30, w.y + 5, "→");
        else
            p.drawText(screenX - 40, w.y + 5, "←");
    }
}

void GameWindow::drawSharks(QPainter& p)
{
    for (auto s : gm->sharks) {
        if (!s->alive) continue;
        int screenX = s->x - gm->cameraX;
        if (screenX < -50 || screenX > 1330) continue;

        p.setBrush(s->isBoss ? QColor(180, 0, 0) : QColor(80, 80, 200));
        p.setPen(Qt::NoPen);
        p.drawEllipse(screenX - 20, s->y - 12, 40, 24);

        p.fillRect(screenX - 20, s->y - 22, 40, 6, QColor(60, 60, 60));
        int bw = (int)(40.0f * s->hp / s->maxHp);
        p.fillRect(screenX - 20, s->y - 22, bw, 6, QColor(220, 50, 50));
    }
}

void GameWindow::drawPlayer(QPainter& p)
{
    int screenX = gm->player->x - gm->cameraX;
    p.setBrush(gm->player->boosting ? QColor(255, 200, 100) : QColor(240, 240, 240));
    p.setPen(QPen(QColor(100, 100, 100), 1));
    p.drawRect(screenX - 20, gm->player->y - 10, 40, 20);
}

void GameWindow::drawHUD(QPainter& p)
{
    p.fillRect(0, 0, 1280, 44, QColor(0, 0, 0, 170));

    p.setFont(QFont("Microsoft YaHei", 10));
    p.setPen(Qt::white);

    // 耐久
    p.drawText(10, 14, "耐久");
    p.fillRect(50, 4, 100, 14, QColor(60, 60, 60));
    int durW = (int)(100.0f * gm->player->durability / gm->player->maxDurability);
    p.fillRect(50, 4, durW, 14, QColor(80, 200, 80));

    // 体力
    p.drawText(165, 14, "体力");
    p.fillRect(205, 4, 100, 14, QColor(60, 60, 60));
    int staW = (int)(100.0f * gm->player->stamina / gm->player->maxStamina);
    p.fillRect(205, 4, staW, 14, QColor(200, 200, 50));

    p.setFont(QFont("Microsoft YaHei", 10));
    p.drawText(315, 28, QString("金币:%1").arg(gm->player->coins));
    p.drawText(410, 28, QString("距离:%1m").arg(gm->player->distance));

    // 时间
    int sec = gm->player->gameSeconds;
    p.drawText(520, 28, QString("时间:%1:%2")
        .arg(sec / 60, 2, 10, QChar('0'))
        .arg(sec % 60, 2, 10, QChar('0')));

    p.drawText(620, 28, QString("击杀:%1").arg(gm->killCount));
    p.drawText(700, 28, QString("捕鱼:%1").arg(gm->player->fishCaught));

    // 武器耐久
    if (gm->currentWeapon) {
        p.drawText(780, 28, QString("%1 耐久:%2/%3")
            .arg(gm->currentWeapon->name)
            .arg(gm->currentWeapon->durability)
            .arg(gm->currentWeapon->maxDurability));
    }

    // 海浪提示
    if (gm->waves.waveActive) {
        p.setPen(QColor(255, 200, 50));
        p.drawText(990, 28, QString("海浪 x%1").arg(
            (double)gm->waves.speedMultiplier, 0, 'f', 1));
    }

    // 右上角进度条
    int stageProgress = std::min(100, (int)(gm->player->distance * 100 / (gm->stage * 2000)));
    p.setPen(Qt::white);
    p.setFont(QFont("Microsoft YaHei", 10));
    p.drawText(1100, 14, QString("关卡 %1/5").arg(gm->stage));
    p.fillRect(1100, 20, 160, 10, QColor(60, 60, 60));
    p.fillRect(1100, 20, stageProgress * 160 / 100, 10, QColor(100, 200, 100));
    p.setPen(QPen(Qt::white, 1));
    p.drawRect(1100, 20, 160, 10);

    p.setPen(QColor(180, 180, 180));
    p.setFont(QFont("Microsoft YaHei", 9));
    p.drawText(1100, 40, "空格:攻击 F:捕鱼 P:商店 ESC:暂停 Q:保存退出");
}

void GameWindow::drawFishingHUD(QPainter& p)
{
    if (!isFishing || !targetFish) return;

    int barX = 490, barY = 55, barW = 300, barH = 22;
    p.fillRect(barX - 5, barY - 22, barW + 10, barH + 28, QColor(0, 0, 0, 180));

    p.setPen(Qt::white);
    p.setFont(QFont("Microsoft YaHei", 11));
    QString fishName;
    switch (targetFish->type) {
    case Fish::SARDINE:       fishName = "沙丁鱼"; break;
    case Fish::TUNA:          fishName = "金枪鱼"; break;
    case Fish::DEEPSEAEEL:    fishName = "深海鳗"; break;
    case Fish::SWORDFISH_FISH: fishName = "金鱼";  break;
    }
    p.drawText(barX, barY - 4, QString("捕捉 %1 — 按F: %2/%3")
        .arg(fishName).arg(fishClickCount).arg(targetFish->catchRequired));

    p.fillRect(barX, barY, barW, barH, QColor(50, 50, 50));
    float ratio = 1.0f - (float)fishTimer / targetFish->catchTimeLimit;
    int fillW = (int)(barW * ratio);
    QColor barColor;
    switch (targetFish->type) {
    case Fish::SARDINE:       barColor = QColor(255, 220, 50);  break;
    case Fish::TUNA:          barColor = QColor(50, 180, 255);  break;
    case Fish::DEEPSEAEEL:    barColor = QColor(180, 50, 255);  break;
    case Fish::SWORDFISH_FISH: barColor = QColor(255, 180, 0); break;
    }
    p.fillRect(barX, barY, fillW, barH, barColor);
    p.setPen(QPen(Qt::white, 1));
    p.drawRect(barX, barY, barW, barH);
}

void GameWindow::drawPaused(QPainter& p)
{
    p.fillRect(0, 0, 1280, 720, QColor(0, 0, 0, 120));
    p.setPen(Qt::white);
    p.setFont(QFont("Microsoft YaHei", 36, QFont::Bold));
    p.drawText(0, 280, 1280, 80, Qt::AlignCenter, "游戏暂停");
    p.setFont(QFont("Microsoft YaHei", 18));
    p.drawText(0, 380, 1280, 40, Qt::AlignCenter, "按 ESC 继续    按 Q 保存退出");
}

void GameWindow::drawDefeat(QPainter& p)
{
    p.fillRect(0, 0, 1280, 720, QColor(80, 0, 0));
    p.setPen(QColor(255, 80, 80));
    p.setFont(QFont("Microsoft YaHei", 72, QFont::Bold));
    p.drawText(0, 200, 1280, 150, Qt::AlignCenter, "DEFEAT");

    p.setPen(Qt::white);
    p.setFont(QFont("Microsoft YaHei", 18));
    p.drawText(0, 380, 1280, 40, Qt::AlignCenter,
        QString("航行距离: %1m   捕鱼: %2条   击杀: %3")
        .arg(gm->player->distance)
        .arg(gm->player->fishCaught)
        .arg(gm->killCount));

    p.setFont(QFont("Microsoft YaHei", 14));
    p.setPen(QColor(255, 200, 80));
    p.drawText(0, 500, 1280, 40, Qt::AlignCenter, "按 Space 重新开始");
}

void GameWindow::drawVictory(QPainter& p)
{
    p.fillRect(0, 0, 1280, 720, QColor(0, 60, 0));
    p.setPen(QColor(100, 255, 100));
    p.setFont(QFont("Microsoft YaHei", 72, QFont::Bold));
    p.drawText(0, 180, 1280, 150, Qt::AlignCenter, "VICTORY");

    int score = gm->player->distance / 10 + gm->player->coins + gm->killCount * 50;
    QString grade;
    if (score >= 800) grade = "S";
    else if (score >= 500) grade = "A";
    else if (score >= 300) grade = "B";
    else                   grade = "C";

    p.setPen(Qt::white);
    p.setFont(QFont("Microsoft YaHei", 24));
    p.drawText(0, 360, 1280, 50, Qt::AlignCenter, QString("评级: %1   得分: %2").arg(grade).arg(score));

    p.setFont(QFont("Microsoft YaHei", 18));
    p.drawText(0, 420, 1280, 40, Qt::AlignCenter,
        QString("航行: %1m   捕鱼: %2条   击杀: %3   用时: %4:%5")
        .arg(gm->player->distance)
        .arg(gm->player->fishCaught)
        .arg(gm->killCount)
        .arg(gm->player->gameSeconds / 60, 2, 10, QChar('0'))
        .arg(gm->player->gameSeconds % 60, 2, 10, QChar('0')));

    p.setPen(QColor(255, 220, 80));
    p.setFont(QFont("Microsoft YaHei", 14));
    p.drawText(0, 520, 1280, 40, Qt::AlignCenter, "按 Space 返回主菜单");
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

    if (fishTimer >= targetFish->catchTimeLimit) {
        targetFish->vx *= 3;
        targetFish->vy *= 3;
        targetFish->escaped = true;
        isFishing = false;
        targetFish = nullptr;
        return;
    }

    if (fishClickCount >= targetFish->catchRequired) {
        targetFish->caught = true;
        gm->player->coins += targetFish->value;
        gm->player->stamina = std::min(gm->player->maxStamina,
            gm->player->stamina + targetFish->staminaGain);
        gm->player->fishCaught++;
        gm->player->fishTotalValue += targetFish->value;
        isFishing = false;
        targetFish = nullptr;
    }
}

void GameWindow::openShop()
{
    ShopDialog dlg(gm->player, this);
    dlg.exec();
}

void GameWindow::keyPressEvent(QKeyEvent* event)
{
    if (state == STATE_INTRO) {
        if (event->key() == Qt::Key_Space) {
            state = STATE_MENU;
            update();
        }
        return;
    }

    if (state == STATE_MENU) {
        if (event->key() == Qt::Key_N) {
            gm->fileManager.deleteSave();
            state = STATE_PLAYING;
        }
        else if (event->key() == Qt::Key_C && gm->fileManager.hasSave()) {
            gm->loadSave();
            state = STATE_PLAYING;
        }
        return;
    }

    if (state == STATE_DEFEAT) {
        if (event->key() == Qt::Key_Space) {
            delete gm;
            gm = new GameManager();
            state = STATE_MENU;
            update();
        }
        return;
    }

    if (state == STATE_VICTORY) {
        if (event->key() == Qt::Key_Space) {
            delete gm;
            gm = new GameManager();
            state = STATE_MENU;
            update();
        }
        return;
    }

    if (state == STATE_PAUSED) {
        if (event->key() == Qt::Key_Escape) {
            state = STATE_PLAYING;
        }
        else if (event->key() == Qt::Key_Q) {
            gm->saveAndQuit();
            close();
        }
        return;
    }

    if (state == STATE_PLAYING) {
        switch (event->key()) {
        case Qt::Key_W: case Qt::Key_Up:    keyUp = true; break;
        case Qt::Key_S: case Qt::Key_Down:  keyDown = true; break;
        case Qt::Key_A: case Qt::Key_Left:  keyLeft = true; break;
        case Qt::Key_D: case Qt::Key_Right: keyRight = true; break;
        case Qt::Key_Shift: keyShift = true; gm->player->boost(); break;
        case Qt::Key_F:
            if (!isFishing) tryStartFishing();
            else fishClickCount++;
            break;
        case Qt::Key_Space:
            gm->attackNearestShark(30, 150);
            break;
        case Qt::Key_P:
            timer->stop();
            openShop();
            timer->start(16);
            break;
        case Qt::Key_Escape:
            state = STATE_PAUSED;
            break;
        case Qt::Key_Q:
            gm->saveAndQuit();
            close();
            break;
        }
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_W: case Qt::Key_Up:    keyUp = false; break;
    case Qt::Key_S: case Qt::Key_Down:  keyDown = false; break;
    case Qt::Key_A: case Qt::Key_Left:  keyLeft = false; break;
    case Qt::Key_D: case Qt::Key_Right: keyRight = false; break;
    case Qt::Key_Shift: keyShift = false; gm->player->stopBoost(); break;
    }
}