#pragma once

#include <string>

#include "core/StateMachine.h"
#include "gameplay/Food.h"
#include "gameplay/Snake.h"
#include "persistence/SaveSystem.h"
#include "systems/InputSystem.h"
#include "systems/RenderSystem.h"
#include "systems/TimeSystem.h"

class GameLoop {
public:
    GameLoop(int boardWidth, int boardHeight, int screenWidth);

    void run();

private:
    int width;
    int height;

    StateMachine fsm;
    Snake snake;
    Food food;
    SaveSystem saveSystem;
    InputSystem inputSystem;
    TimeSystem timeSystem;
    RenderSystem renderSystem;

    int score;

    void processInput();
    void update();
    void render();

    void resetGame();
    bool isWallCollision(const Point& head) const;
    std::wstring statusLine() const;

    SaveData buildSaveData() const;
    bool restoreFromSave();
};
