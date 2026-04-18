#pragma once
#include "Player.h"

class WaveSystem {
public:
    WaveSystem();
    void update(Player& player);

    float speedMultiplier = 1.0f;
    bool waveActive = false;
    int waveTimer = 0;
    int nextWaveIn = 300;
    int waveDuration = 120;
    float targetMultiplier = 1.0f;
};
