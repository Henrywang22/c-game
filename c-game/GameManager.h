#pragma once
#include <vector>
#include "Player.h"
#include "Fish.h"
#include "Obstacle.h"
#include "Shark.h"
#include "WaveSystem.h"
#include "FileManager.h"

class GameManager {
public:
    GameManager();
    ~GameManager();

    void update();
    void spawnFish();
    void spawnObstacles();
    void spawnShark();
    void spawnBoss(int stage);
    void checkCollisions();
    void tryFish();
    void attackNearestShark(int damage, int range);
    bool isBossDefeated();

    Player* player;
    std::vector<Fish*>     fish;
    std::vector<Obstacle*> obstacles;
    std::vector<Shark*>    sharks;
    WaveSystem             waves;
    FileManager            fileManager;

    int stage = 1;
    int killCount = 0;
    bool bossSpawned = false;
    bool gameOver = false;
    bool stageClear = false;

private:
    int spawnTimer = 0;
};
