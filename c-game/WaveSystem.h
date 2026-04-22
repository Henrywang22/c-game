#pragma once
#include <vector>
#include <algorithm>
#include "Player.h"

enum WaveDirection { WAVE_LEFT, WAVE_RIGHT };

struct WaveObj {
    int x, y;
    WaveDirection dir;
    int timer = 0;
    bool active = true;
};

class WaveSystem {
public:
    WaveSystem();
    void update(Player& player);
    void spawnWave();

    float speedMultiplier = 1.0f;
    bool waveActive = false;
    int waveTimer = 0;
    int nextWaveIn = 300;
    float targetMultiplier = 1.0f;

    std::vector<WaveObj> waves;
};
