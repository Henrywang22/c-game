#include "WeatherSystem.h"
#include <QPainter>
#include <QFont>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

WeatherSystem::WeatherSystem() {}

void WeatherSystem::update(Player& player)
{
    weatherTimer++;

    // 天气切换
    if (weatherTimer >= nextChangeIn) {
        weatherTimer = 0;
        int r = rand() % 10;
        if (r < 5) current = WEATHER_CLEAR;
        else if (r < 8) current = WEATHER_FOG;
        else            current = WEATHER_STORM;

        duration = 300 + rand() % 400;
        nextChangeIn = duration;

        // 重置
        fogAlpha = 0;
        lightnings.clear();
    }

    // 雾效果
    if (current == WEATHER_FOG) {
        fogAlpha = std::min(180.0f, fogAlpha + 2.0f);
    }
    else {
        fogAlpha = std::max(0.0f, fogAlpha - 3.0f);
    }

    // 暴风雨效果
    if (current == WEATHER_STORM) {
        // 降低船速
        player.speed = std::max(1, (int)(player.speed * 0.8f));

        // 随机闪电
        lightningTimer++;
        if (lightningTimer >= 60 + rand() % 120) {
            lightningTimer = 0;

            // 生成闪电
            Lightning lt;
            lt.x = rand() % 1280;
            lt.y1 = 0;
            lt.y2 = 80 + rand() % 500;
            lt.life = 10;
            lightnings.push_back(lt);

            // 闪电在玩家附近则扣耐久
            int dx = player.x - lt.x;
            if (abs(dx) < 100) {
                player.durability -= 15;
                if (player.durability < 0) player.durability = 0;
            }
        }

        // 向左触碰边界死亡（暴风雨特有）
        if (player.x <= 20) {
            player.durability = 0;
        }

        // 更新闪电生命周期
        for (auto& lt : lightnings) lt.life--;
        lightnings.erase(
            std::remove_if(lightnings.begin(), lightnings.end(),
                [](const Lightning& lt) { return lt.life <= 0; }),
            lightnings.end());
    }
}

void WeatherSystem::draw(QPainter& p, int cameraX)
{
    // 雾效果
    if (fogAlpha > 0) {
        p.fillRect(0, 44, 1280, 676, QColor(200, 200, 220, (int)fogAlpha));

        // 雾中只显示玩家附近区域（模拟视野缩小）
        p.setFont(QFont("Microsoft YaHei", 14));
        p.setPen(QColor(150, 150, 180));
        p.drawText(0, 680, 1280, 30, Qt::AlignCenter, "浓雾警告：视野受限");
    }

    // 暴风雨效果
    if (current == WEATHER_STORM) {
        // 雨滴
        p.setPen(QPen(QColor(150, 180, 255, 120), 1));
        for (int i = 0; i < 80; i++) {
            int rx = rand() % 1280;
            int ry = rand() % 720;
            p.drawLine(rx, ry, rx - 3, ry + 12);
        }

        // 闪电
        for (auto& lt : lightnings) {
            p.setPen(QPen(QColor(255, 255, 150, 200 * lt.life / 10), 2));
            p.drawLine(lt.x, lt.y1, lt.x + (rand() % 20 - 10), lt.y2);

            // 闪电光晕
            p.fillRect(lt.x - 60, 0, 120, 720,
                QColor(255, 255, 200, 30 * lt.life / 10));
        }

        // 暴风雨提示
        p.setFont(QFont("Microsoft YaHei", 13));
        p.setPen(QColor(255, 150, 150));
        p.drawText(0, 680, 1280, 30, Qt::AlignCenter, QString::fromUtf8("暴风雨！向左触碰边界将直接死亡！"));

        // 暗色滤镜
        p.fillRect(0, 0, 1280, 720, QColor(0, 0, 40, 60));
    }
}