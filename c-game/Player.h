#pragma once

class Player {
public:
    Player(int startX, int startY);
    void move(int dx, int dy);
    void boost();       // Shift加速
    void stopBoost();   // 松开Shift

    int x, y;
    int durability;
    int maxDurability;
    int stamina;
    int maxStamina;
    int coins;
    int distance;
    int speed;
    int baseSpeed;
    bool boosting = false;

    // 统计
    int fishCaught = 0;
    int fishTotalValue = 0;
    int gameSeconds = 0;

    void move(int dx, int dy, int cameraX = 0);
};