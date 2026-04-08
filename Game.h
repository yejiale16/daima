#pragma once

#include <string>

#include "Food.h"
#include "Input.h"
#include "Renderer.h"
#include "SaveSystem.h"
#include "Snake.h"

class Game {
public:
    Game(int width, int height);

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

    void resetGame();
    void processInput(const InputCommand& cmd);
    void updateLogic();
    std::string buildInfoText() const;

    bool isWallCollision(const Point& head) const;

    SavedGameData captureSaveData() const;
    bool restoreFromSave();
};
