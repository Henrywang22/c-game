#pragma once
#include <QRect>

class Fish {
public:
    enum Type { EDIBLE, RARE };

    Fish(int x, int y, Type type);
    void update();
    bool isNearPlayer(int px, int py, int range);

    int x, y;
    int vx, vy;
    Type type;
    int value;
    int staminaGain;
    bool caught = false;
    bool escaped = false;  // 捕捉失败后逃走

    // 捕捉难度相关
    int catchRequired;   // 需要按键次数
    int catchTimeLimit;  // 时间限制（帧数）

private:
    int moveTimer = 0;
};