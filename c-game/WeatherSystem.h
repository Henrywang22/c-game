#pragma once
#include <vector>
#include <algorithm>
#include "Player.h"

enum WeatherType { WEATHER_CLEAR, WEATHER_FOG, WEATHER_STORM };

class WeatherSystem {
public:
    WeatherSystem();
    void update(Player& player);
    void draw(class QPainter& p, int cameraX);

    WeatherType current = WEATHER_CLEAR;
    int weatherTimer = 0;
    int nextChangeIn = 600;
    int duration = 0;

    // ŒÌ
    float fogAlpha = 0;

    // ±©∑Á”Í
    int lightningTimer = 0;
    int lightningFlash = 0;
    struct Lightning {
        int x, y1, y2;
        int life;
    };
    std::vector<Lightning> lightnings;

    bool isStormy() const { return current == WEATHER_STORM; }
    bool isFoggy()  const { return current == WEATHER_FOG; }
};
