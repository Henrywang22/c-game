# Fishing Voyage (渔途)

A 2D top-down ocean survival game built with C++ and Qt framework.

## Gameplay

Sail your ship rightward across 5 stages, each approximately 8 minutes long.
Catch fish, fight sharks, avoid obstacles, and defeat the Boss Shark at the end of each stage.

## Controls

| Key | Action |
|-----|--------|
| W / A / S / D or Arrow Keys | Move ship |
| Space | Attack nearest shark |
| P | Open shop |

## Features

- **Fishing system** — Automatically catch fish when in range. Yellow fish restore more stamina, red fish are worth more coins.
- **Obstacle system** — Reefs and whirlpools are hidden until you get close. Reefs damage durability, whirlpools drain stamina.
- **Shark combat** — Normal sharks patrol the sea. Boss sharks appear at the end of each stage and enter an enraged state at 50% HP.
- **Wave system** — Random waves temporarily boost or reduce ship speed (0.5x to 1.5x).
- **Shop** — Buy food, repair kits, and weapons between stages using coins earned from fishing and kills.
- **File I/O** — Game progress saved to `save.dat`, leaderboard stored in `highscore.dat`, fish logbook in `Log.dat`.
- **Scoring** — Final grade (S / A / B / C) based on distance sailed, coins earned, and kill count.

## OOP Structure

Three inheritance chains implemented:

- `Fish` → `CommonFish` / `RareFish` → `Sardine`, `Tuna`, `DeepSeaEel`, `Shark`
- `Obstacle` → `StaticObstacle` / `ActiveObstacle` → `Reef`, `Whirlpool`, `SharkAttack`
- `Item` → `Weapon`, `FoodItem`, `RepairKit`

## Tech Stack

- Language: C++
- UI Framework: Qt 6.x (QPainter, QWidget, QTimer, QDialog)
- IDE: Visual Studio 2022
- Version Control: Git / GitHub

## Team

5-member group project for C++ course assignment.

## Build

1. Install Qt 6.x with MSVC compiler
2. Install Qt Visual Studio Tools extension
3. Open the `.vcxproj` file in Visual Studio
4. Build and run
