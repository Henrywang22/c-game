#pragma once
#include <QRect>

class Fish {
public:
    enum Type { EDIBLE, RARE };

    Fish(int x, int y, Type type);
    void update();
    bool isCaught(int px, int py, int range);

    int x, y;
    int vx, vy;
    Type type;
    int value;        // 卖出金币
    int staminaGain;  // 吃掉回复体力
    bool caught = false;

private:
    int moveTimer = 0;
};
