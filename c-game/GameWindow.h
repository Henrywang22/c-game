#pragma once
#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include "GameManager.h"

class GameWindow : public QWidget
{
    Q_OBJECT
public:
    GameWindow(QWidget* parent = nullptr);
    ~GameWindow();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void gameLoop();

private:
    QTimer* timer;
    GameManager* gm;

    bool keyUp = false;
    bool keyDown = false;
    bool keyLeft = false;
    bool keyRight = false;

    void drawSea(QPainter& p);
    void drawFish(QPainter& p);
    void drawObstacles(QPainter& p);
    void drawSharks(QPainter& p);
    void drawPlayer(QPainter& p);
    void drawHUD(QPainter& p);
    void openShop();
    void showResult();

    bool showIntro = true;  // 是否显示开始介绍界面
    void drawIntro(QPainter& p);

    // 捕鱼系统
    Fish* targetFish = nullptr;  // 当前瞄准的鱼
    bool isFishing = false;      // 是否正在捕鱼
    int fishClickCount = 0;      // 当前点击次数
    int fishTimer = 0;           // 捕鱼计时器
    void tryStartFishing();      // 尝试开始捕鱼
    void updateFishing();        // 更新捕鱼状态
    void drawFishingHUD(QPainter& p); // 画捕鱼进度条
};
