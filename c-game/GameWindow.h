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
};
