#pragma once

#include <optional>

#include "core/StateMachine.h"
#include "gameplay/Snake.h"

struct InputFrame {
    std::optional<Direction> latestLegalDirection;
    bool start = false;
    bool pauseToggle = false;
    bool save = false;
    bool load = false;
    bool quit = false;
};

class InputSystem {
public:
    InputFrame processInput(Direction currentDirection, GameState state);

private:
    bool isDirectionKey(int key) const;
    std::optional<Direction> toDirection(int key) const;
    int readSingleKey() const;
};
