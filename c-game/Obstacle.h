#pragma once

enum ObstacleType { REEF, WHIRLPOOL };

class Obstacle {
public:
    Obstacle(int x, int y, ObstacleType type);
    virtual void triggerEffect(class Player& player) = 0;
    bool isVisible(int px, int py);

    int x, y;
    ObstacleType type;
    bool visible = false;
    int revealRange = 180;
};

class Reef : public Obstacle {
public:
    Reef(int x, int y);
    void triggerEffect(Player& player) override;
};

class Whirlpool : public Obstacle {
public:
    Whirlpool(int x, int y);
    void triggerEffect(Player& player) override;
};
