#pragma once

class Player
{
public:
    Player(int startX, int startY);

    void move(int dx, int dy);

    int x, y;           // 位置
    int durability;     // 耐久度
    int stamina;        // 体力
    int coins;          // 金币
    int distance;       // 已航行距离

    int speed = 3;      // 移动速度
}; 
