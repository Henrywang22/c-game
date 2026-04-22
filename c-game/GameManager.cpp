#include "GameManager.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

GameManager::GameManager()
{
    player = new Player(100, 360);
    currentWeapon = new Harpoon();
    spawnObstacles();
    for (int i = 0; i < 5; i++) spawnFish();
}

GameManager::~GameManager()
{
    delete player;
    delete currentWeapon;
    for (auto f : fish)      delete f;
    for (auto o : obstacles) delete o;
    for (auto s : sharks)    delete s;
}

void GameManager::update()
{
    if (gameOver || victory) return;

    gameTimer++;
    if (gameTimer % 60 == 0) player->gameSeconds++;

    // 清理鱼
    fish.erase(std::remove_if(fish.begin(), fish.end(),
        [](Fish* f) {
            if (f->caught || f->escaped) { delete f; return true; }
            return false;
        }), fish.end());

    // 更新
    waves.update(*player);
    weather.update(*player);
    for (auto f : fish)   f->update(player->x, player->y);
    for (auto s : sharks) s->update(*player);
    for (auto o : obstacles) o->isVisible(player->x - cameraX, player->y);

    // 卷轴
   // 船保持在屏幕左侧1/3位置
    cameraX = player->x - 300;
    if (cameraX < 0) cameraX = 0;

    spawnTimer++;

    // 补鱼
    int aliveFish = 0;
    for (auto f : fish)
        if (!f->caught && !f->escaped) aliveFish++;
    if (spawnTimer % 300 == 0 && aliveFish < 5) spawnFish();

    // 鲨鱼
    if (spawnTimer % 400 == 0 && !bossSpawned) spawnShark();

    // Boss触发（每关2000距离）
    if (player->distance > stage * 2000 && !bossSpawned) {
        spawnBoss(stage);
        bossSpawned = true;
    }

    checkCollisions();

    if (player->durability <= 0) gameOver = true;
    if (player->stamina <= 0)    player->speed = 1;

    // 通关判定
    if (stage > 5) victory = true;
}

void GameManager::spawnFish()
{
    int x = player->x + 300 + rand() % 600;
    int y = 80 + rand() % 580;
    int r = rand() % 10;
    Fish* f;
    if (r < 4) f = new Sardine(x, y);
    else if (r < 7) f = new Tuna(x, y);
    else if (r < 9) f = new DeepSeaEel(x, y);
    else            f = new GoldenFish(x, y);
    fish.push_back(f);
}

void GameManager::spawnObstacles()
{
    // 逐关递增陷阱数
    int count = 6 + stage * 2;
    for (int i = 0; i < count; i++) {
        int x = 400 + rand() % (stage * 800 + 800);
        int y = 80 + rand() % 580;
        if (rand() % 3 == 0)
            obstacles.push_back(new Whirlpool(x, y));
        else
            obstacles.push_back(new Reef(x, y));
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
    // 障碍物
    for (auto o : obstacles) {
        if (!o->visible) continue;
        int dx = player->x - o->x;
        int dy = player->y - o->y;
        int dist2 = dx * dx + dy * dy;
        int r = o->size + 20;
        if (dist2 < r * r) {
            o->triggerEffect(*player);
            if (o->type == REEF) {
                Reef* reef = static_cast<Reef*>(o);
                reef->applyRebound(*player, (float)player->speed);
            }
        }
    }

    // 鲨鱼
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

    // Boss击败 → 下一关
    if (bossSpawned) {
        for (auto s : sharks) {
            if (s->isBoss && !s->alive) {
                stageClear = true;
            }
        }
    }
}

void GameManager::attackNearestShark(int damage, int range)
{
    if (currentWeapon && currentWeapon->durability <= 0) return;

    int actualDamage = currentWeapon ? currentWeapon->fire() : damage;

    Shark* nearest = nullptr;
    float minDist = (float)(range * range);
    for (auto s : sharks) {
        if (!s->alive) continue;
        float dx = (float)(player->x - s->x);
        float dy = (float)(player->y - s->y);
        float dist = dx * dx + dy * dy;
        if (dist < minDist) { minDist = dist; nearest = s; }
    }
    if (nearest) {
        nearest->hp -= actualDamage;
        if (nearest->hp <= 0) {
            nearest->alive = false;
            player->coins += nearest->dropValue;
            killCount++;
        }
    }
}

void GameManager::saveAndQuit()
{
    SaveData data;
    data.stage = stage;
    data.distance = player->distance;
    data.coins = player->coins;
    data.durability = player->durability;
    data.stamina = player->stamina;
    data.fishCaught = player->fishCaught;
    data.fishTotalValue = player->fishTotalValue;
    data.gameSeconds = player->gameSeconds;
    data.isDead = false;
    fileManager.saveGame(data);
}

void GameManager::loadSave()
{
    SaveData data;
    if (fileManager.loadGame(data) && !data.isDead) {
        stage = data.stage;
        player->distance = data.distance;
        player->coins = data.coins;
        player->durability = data.durability;
        player->stamina = data.stamina;
        player->fishCaught = data.fishCaught;
        player->fishTotalValue = data.fishTotalValue;
        player->gameSeconds = data.gameSeconds;
    }
}

bool GameManager::isBossDefeated()
{
    for (auto s : sharks)
        if (s->isBoss && !s->alive) return true;
    return false;
}