#include "WaveSystem.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

WaveSystem::WaveSystem() {}

void WaveSystem::spawnWave()
{
    WaveObj w;
    w.x = (rand() % 2 == 0) ? 0 : 1280;
    w.y = 80 + rand() % 580;
    w.dir = (rand() % 2 == 0) ? WAVE_LEFT : WAVE_RIGHT;
    w.timer = 0;
    w.active = true;
    waves.push_back(w);
}

void WaveSystem::update(Player& player)
{
    waveTimer++;

    if (waveTimer >= nextWaveIn) {
        spawnWave();
        waveTimer = 0;
        nextWaveIn = 200 + rand() % 300;
    }

    for (auto& w : waves) {
        if (!w.active) continue;
        w.timer++;

        // 海浪移动
        if (w.dir == WAVE_RIGHT) w.x += 3;
        else w.x -= 3;

        // 检测是否影响玩家
        int dx = player.x - w.x;
        int dy = player.y - w.y;
        if (abs(dx) < 100 && abs(dy) < 80) {
            // 顺风加速 逆风减速
            bool tailwind = (w.dir == WAVE_RIGHT && dx > 0) ||
                (w.dir == WAVE_LEFT && dx < 0);
            speedMultiplier = tailwind ? 1.5f : 0.6f;
            waveActive = true;
        }

        if (w.x < -100 || w.x > 1400) w.active = false;
    }

    // 清理
    waves.erase(std::remove_if(waves.begin(), waves.end(),
        [](const WaveObj& w) { return !w.active; }), waves.end());

    if (!waveActive) {
        speedMultiplier += (1.0f - speedMultiplier) * 0.05f;
    }
    waveActive = false;

    player.speed = (int)(player.baseSpeed * speedMultiplier);
    if (player.speed < 1) player.speed = 1;
    if (player.speed > 8) player.speed = 8;
}