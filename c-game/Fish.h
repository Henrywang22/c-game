#pragma once

class Fish {
public:
    enum Type { SARDINE, TUNA, DEEPSEAEEL };

    Fish(int x, int y, Type type);
    void update();
    bool isNearPlayer(int px, int py, int range);
    virtual int getEconomicValue() = 0;
    virtual ~Fish() {}

    int x, y;
    int vx, vy;
    Type type;
    int value;
    int staminaGain;
    int catchRequired;
    int catchTimeLimit;
    bool caught = false;
    bool escaped = false;

protected:
    int moveTimer = 0;
};

class CommonFish : public Fish {
public:
    CommonFish(int x, int y, Type type);
};

class RareFish : public Fish {
public:
    RareFish(int x, int y, Type type);
};

class Sardine : public CommonFish {
public:
    Sardine(int x, int y);
    int getEconomicValue() override;
};

class Tuna : public CommonFish {
public:
    Tuna(int x, int y);
    int getEconomicValue() override;
};

class DeepSeaEel : public RareFish {
public:
    DeepSeaEel(int x, int y);
    int getEconomicValue() override;
};