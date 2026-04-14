#pragma once

#include <string>

#include "Food.h"
#include "Input.h"
#include "Renderer.h"
#include "SaveSystem.h"
#include "Snake.h"

class Game {
public:
    Game(int width, int height, int screenWidth);

    void run();

private:
    int width;
    int height;

    bool running;
    int score;
    GameStatus status;

    Snake snake;
    Food food;
    Input input;
    Renderer renderer;
    SaveSystem saveSystem;
    int currentDifficulty;
    int currentSpeedMs;
    double currentSnakeSpeed;

    void resetGame();
    void processInput(const InputCommand& cmd);
    void updateLogic();
    std::wstring buildInfoText() const;

    bool isWallCollision(const Point& head) const;
    int computeDifficulty() const;
    int computeStepIntervalMs(int difficulty) const;
    double computeSnakeSpeed(int stepIntervalMs) const;

    SavedGameData captureSaveData() const;
    bool restoreFromSave();
};
