#pragma once
#include "Enemy.h"
#include <vector>

class Boss : public Enemy {
public:
    enum State { PHASE1, PHASE2 };

    Boss(int x, int y);
    void update(Player& player) override;
    bool collidesWithPlayer(int px, int py) override;
    void spawnMinions(std::vector<class Shark*>& sharks);

    State state = PHASE1;
    bool minionSpawned = false;
    int phaseTimer = 0;
};
