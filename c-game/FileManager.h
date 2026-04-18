#pragma once
#include <QString>
#include <vector>

struct HighScoreEntry {
    char name[20];
    int score;
    int distance;
    int kills;
};

struct FishEntry {
    int fishID;
    bool discovered;
    char name[30];
};

class FileManager {
public:
    FileManager();
    void saveGame(int distance, int coins, int durability, int stamina);
    bool loadGame(int& distance, int& coins, int& durability, int& stamina);
    void markFishDiscovered(int fishID, const char* fishName);
    bool isFishDiscovered(int fishID);
    void saveHighScore(const char* name, int score, int distance, int kills);
    std::vector<HighScoreEntry> loadHighScores();
};
