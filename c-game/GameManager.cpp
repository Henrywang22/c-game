#include "GameManager.h"
#include <cstdlib>
#include <cmath>

GameManager::GameManager()
{
    player = new Player(100, 360);
    spawnObstacles();
    spawnFish();
}

GameManager::~GameManager()
{
    delete player;
    for (auto f : fish)      delete f;
    for (auto o : obstacles) delete o;
    for (auto s : sharks)    delete s;
}

void GameManager::update()
{
    if (gameOver) return;

    waves.update(*player);

    for (auto f : fish)  f->update();
    for (auto s : sharks) s->update(*player);

    // 更新障碍物可见性
    for (auto o : obstacles)
        o->isVisible(player->x, player->y);

    spawnTimer++;
    if (spawnTimer % 300 == 0) spawnFish();
    if (spawnTimer % 600 == 0 && !bossSpawned) spawnShark();

    // 每关 8 分钟 = 约 28800 帧 触发 Boss
    if (player->distance > stage * 2000 && !bossSpawned) {
        spawnBoss(stage);
        bossSpawned = true;
    }

    checkCollisions();

    if (player->durability <= 0) gameOver = true;
    if (player->stamina <= 0)    player->speed = 1;
}

void GameManager::spawnFish()
{
    for (int i = 0; i < 5; i++) {
        int x = 200 + rand() % 1000;
        int y = 80 + rand() % 580;
        Fish::Type t = (rand() % 3 == 0) ? Fish::RARE : Fish::EDIBLE;
        fish.push_back(new Fish(x, y, t));
    }
}

void GameManager::spawnObstacles()
{
    // 固定位置障碍物
    int positions[][2] = {
        {400,200},{600,450},{800,300},{1000,500},
        {1200,200},{1400,400},{1600,300},{1800,500}
    };
    for (auto& p : positions) {
        if (rand() % 2 == 0)
            obstacles.push_back(new Reef(p[0], p[1]));
        else
            obstacles.push_back(new Whirlpool(p[0], p[1]));
    }
}

void GameManager::spawnShark()
{
    int x = player->x + 400 + rand() % 200;
    int y = 80 + rand() % 580;
    sharks.push_back(new Shark(x, y, false));
}

void GameManager::spawnBoss(int stage)
{
    int x = player->x + 500;
    int y = 360;
    sharks.push_back(new Shark(x, y, true));
}

void GameManager::checkCollisions()
{
    // 捕鱼：自动范围捕捉
    for (auto f : fish) {
        if (!f->caught && f->isCaught(player->x, player->y, 60)) {
            f->caught = true;
            player->coins += f->value;
            player->stamina = std::min(100, player->stamina + f->staminaGain);
        }
    }

    // 障碍物碰撞
    for (auto o : obstacles) {
        if (o->visible) {
            int dx = player->x - o->x;
            int dy = player->y - o->y;
            if (dx * dx + dy * dy < 35 * 35)
                o->triggerEffect(*player);
        }
    }

    // 鲨鱼碰撞
    for (auto s : sharks) {
        if (s->alive && s->collidesWithPlayer(player->x, player->y)) {
            player->durability -= s->attack / 30;
            if (player->durability < 0) player->durability = 0;
        }
    }

    // Boss 击败判定
    if (bossSpawned) {
        for (auto s : sharks) {
            if (s->isBoss && !s->alive) {
                stageClear = true;
            }
        }
    }
}

void GameManager::tryFish() {}

void GameManager::attackNearestShark(int damage, int range)
{
    Shark* nearest = nullptr;
    float minDist = (float)(range * range);

    for (auto s : sharks) {
        if (!s->alive) continue;
        float dx = (float)(player->x - s->x);
        float dy = (float)(player->y - s->y);
        float dist = dx * dx + dy * dy;
        if (dist < minDist) {
            minDist = dist;
            nearest = s;
        }
    }

    if (nearest) {
        nearest->hp -= damage;
        if (nearest->hp <= 0) {
            nearest->alive = false;
            player->coins += nearest->dropValue;
            killCount++;
        }
    }
}

bool GameManager::isBossDefeated()
{
    for (auto s : sharks)
        if (s->isBoss && !s->alive) return true;
    return false;
}