#include "GameManager.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

GameManager::GameManager()
{
    player = new Player(100, 360);
    spawnObstacles();
    for (int i = 0; i < 5; i++) spawnFish();
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

    // 清理鱼 — 加在最开头
    fish.erase(std::remove_if(fish.begin(), fish.end(),
        [](Fish* f) {
            if (f->caught || f->escaped) { delete f; return true; }
            return false;
        }), fish.end());

    waves.update(*player);
    for (auto f : fish)  f->update();
    for (auto s : sharks) s->update(*player);
    for (auto o : obstacles) o->isVisible(player->x, player->y);

    spawnTimer++;

    // 刷鱼 — 替换原来的 if (spawnTimer % 300 == 0) spawnFish();
    int aliveFish = 0;
    for (auto f : fish) {
        if (!f->caught && !f->escaped) aliveFish++;
    }
    if (spawnTimer % 300 == 0 && aliveFish < 5) spawnFish();

    // 鲨鱼频率 — 把600改成400
    if (spawnTimer % 400 == 0 && !bossSpawned) spawnShark();

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
    int x = player->x + 100 + rand() % 600;
    int y = 80 + rand() % 580;
    int r = rand() % 10;
    Fish* f;
    if (r < 5)      f = new Sardine(x, y);       // 50% 沙丁鱼
    else if (r < 8) f = new Tuna(x, y);           // 30% 金枪鱼
    else            f = new DeepSeaEel(x, y);      // 20% 深海鳗
    fish.push_back(f);
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
    int x = player->x + 200 + rand() % 150;
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
            s->attackTimer++;
            if (s->attackTimer >= 60) {
                player->durability -= s->attack;
                if (player->durability < 0) player->durability = 0;
                s->attackTimer = 0;
            }
        }
        else {
            s->attackTimer = 0;
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