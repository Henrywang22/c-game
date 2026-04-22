#pragma once
#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include "GameManager.h"
#include "NormalEnemy.h"
#include "Boss.h"

enum GameState {
    STATE_MENU, STATE_INTRO, STATE_PLAYING, STATE_PAUSED,
    STATE_SHOP, STATE_DEFEAT, STATE_VICTORY
};

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
    GameState    state = STATE_INTRO;

    bool keyUp = false;
    bool keyDown = false;
    bool keyLeft = false;
    bool keyRight = false;
    bool keyShift = false;

    // ²¶Óã
    Fish* targetFish = nullptr;
    bool  isFishing = false;
    int   fishClickCount = 0;
    int   fishTimer = 0;

    // »æÖÆº¯Êý
    void drawIntro(QPainter& p);
    void drawMenu(QPainter& p);
    void drawGame(QPainter& p);
    void drawSea(QPainter& p);
    void drawFish(QPainter& p);
    void drawObstacles(QPainter& p);
    void drawSharks(QPainter& p);
    void drawPlayer(QPainter& p);
    void drawWaves(QPainter& p);
    void drawHUD(QPainter& p);
    void drawFishingHUD(QPainter& p);
    void drawPaused(QPainter& p);
    void drawDefeat(QPainter& p);
    void drawVictory(QPainter& p);

    void tryStartFishing();
    void updateFishing();
    void openShop();
};