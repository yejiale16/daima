#pragma once

#include <array>
#include <optional>

#include "Snake.h"

struct InputCommand {
    std::optional<Direction> direction;
    bool quit = false;
    bool pauseToggle = false;
    bool save = false;
    bool load = false;
    bool start = false;
};

class Input {
public:
    Input();

    InputCommand poll();

private:
    std::array<bool, 256> previousState;

    bool pressedOnce(int vk);
};
