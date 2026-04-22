#pragma once

enum ObstacleType { REEF, WHIRLPOOL };

class Player;

class Obstacle {
public:
    Obstacle(int x, int y, ObstacleType type);
    virtual void triggerEffect(Player& player) = 0;
    virtual ~Obstacle() {}
    bool isVisible(int px, int py);

    int x, y;
    ObstacleType type;
    bool visible = false;
    int revealRange = 180;
    int size;
};

class Reef : public Obstacle {
public:
    Reef(int x, int y);
    void triggerEffect(Player& player) override;
    // ·´µ¯
    void applyRebound(Player& player, float collisionSpeed);
};

class Whirlpool : public Obstacle {
public:
    Whirlpool(int x, int y);
    void triggerEffect(Player& player) override;
};