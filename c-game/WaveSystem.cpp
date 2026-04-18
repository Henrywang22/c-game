#include "WaveSystem.h"
#include <cstdlib>
#include <cmath>

WaveSystem::WaveSystem() {}

void WaveSystem::update(Player& player)
{
    waveTimer++;

    if (!waveActive && waveTimer >= nextWaveIn) {
        waveActive = true;
        waveTimer = 0;
        targetMultiplier = 0.5f + (rand() % 11) * 0.1f;
        nextWaveIn = 200 + rand() % 300;
    }

    if (waveActive) {
        speedMultiplier += (targetMultiplier - speedMultiplier) * 0.05f;
        if (waveTimer >= waveDuration) {
            waveActive = false;
            waveTimer = 0;
            targetMultiplier = 1.0f;
        }
    }
    else {
        speedMultiplier += (1.0f - speedMultiplier) * 0.05f;
    }

    player.speed = (int)(3 * speedMultiplier);
    if (player.speed < 1) player.speed = 1;
}