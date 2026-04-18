#define _CRT_SECURE_NO_WARNINGS
#include "FileManager.h"
#include <fstream>
#include <cstring>
#include <algorithm>

FileManager::FileManager()
{
    // 初始化图鉴文件
    std::fstream f("Log.dat", std::ios::binary | std::ios::in);
    if (!f.is_open()) {
        std::ofstream init("Log.dat", std::ios::binary);
        FishEntry empty = { 0, false, "" };
        for (int i = 0; i < 10; i++) {
            empty.fishID = i;
            init.write(reinterpret_cast<char*>(&empty), sizeof(FishEntry));
        }
    }
}

void FileManager::saveGame(int distance, int coins, int durability, int stamina)
{
    std::ofstream f("save.dat", std::ios::binary);
    f.write(reinterpret_cast<char*>(&distance), sizeof(int));
    f.write(reinterpret_cast<char*>(&coins), sizeof(int));
    f.write(reinterpret_cast<char*>(&durability), sizeof(int));
    f.write(reinterpret_cast<char*>(&stamina), sizeof(int));
}

bool FileManager::loadGame(int& distance, int& coins, int& durability, int& stamina)
{
    std::ifstream f("save.dat", std::ios::binary);
    if (!f.is_open()) return false;
    f.read(reinterpret_cast<char*>(&distance), sizeof(int));
    f.read(reinterpret_cast<char*>(&coins), sizeof(int));
    f.read(reinterpret_cast<char*>(&durability), sizeof(int));
    f.read(reinterpret_cast<char*>(&stamina), sizeof(int));
    return true;
}

void FileManager::markFishDiscovered(int fishID, const char* fishName)
{
    std::fstream f("Log.dat", std::ios::binary | std::ios::in | std::ios::out);
    f.seekp(fishID * sizeof(FishEntry));
    FishEntry entry;
    f.read(reinterpret_cast<char*>(&entry), sizeof(FishEntry));
    entry.discovered = true;
    strncpy(entry.name, fishName, 29);
    f.seekp(fishID * sizeof(FishEntry));
    f.write(reinterpret_cast<char*>(&entry), sizeof(FishEntry));
}

bool FileManager::isFishDiscovered(int fishID)
{
    std::ifstream f("Log.dat", std::ios::binary);
    f.seekg(fishID * sizeof(FishEntry));
    FishEntry entry;
    f.read(reinterpret_cast<char*>(&entry), sizeof(FishEntry));
    return entry.discovered;
}

void FileManager::saveHighScore(const char* name, int score, int distance, int kills)
{
    std::vector<HighScoreEntry> scores = loadHighScores();
    HighScoreEntry newEntry;
    strncpy(newEntry.name, name, 19);
    newEntry.score = score;
    newEntry.distance = distance;
    newEntry.kills = kills;
    scores.push_back(newEntry);
    std::sort(scores.begin(), scores.end(),
        [](const HighScoreEntry& a, const HighScoreEntry& b) { return a.score > b.score; });
    if (scores.size() > 10) scores.resize(10);

    std::ofstream f("highscore.dat", std::ios::binary);
    for (auto& e : scores)
        f.write(reinterpret_cast<char*>(&e), sizeof(HighScoreEntry));
}

std::vector<HighScoreEntry> FileManager::loadHighScores()
{
    std::vector<HighScoreEntry> scores;
    std::ifstream f("highscore.dat", std::ios::binary);
    if (!f.is_open()) return scores;
    HighScoreEntry entry;
    while (f.read(reinterpret_cast<char*>(&entry), sizeof(HighScoreEntry)))
        scores.push_back(entry);
    return scores;
}
