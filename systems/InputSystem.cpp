#include "systems/InputSystem.h"

#include <conio.h>

InputFrame InputSystem::processInput(Direction currentDirection, GameState state) {
    InputFrame frame;
    Direction nextDirection = currentDirection;
    bool hasLegalDirection = false;

    while (_kbhit()) {
        const int key = readSingleKey();

        if (key == 'q' || key == 'Q' || key == 27) {
            frame.quit = true;
            continue;
        }
        if (key == 'p' || key == 'P') {
            frame.pauseToggle = true;
            continue;
        }
        if (key == 'k' || key == 'K') {
            frame.save = true;
            continue;
        }
        if (key == 'l' || key == 'L') {
            frame.load = true;
            continue;
        }
        if (key == ' ') {
            frame.start = true;
            continue;
        }

        if (state != GameState::RUNNING || !isDirectionKey(key)) {
            continue;
        }

        const std::optional<Direction> inputDirection = toDirection(key);
        if (!inputDirection.has_value()) {
            continue;
        }

        if (!Snake::isOpposite(*inputDirection, currentDirection)) {
            nextDirection = *inputDirection;
            hasLegalDirection = true;
        }
    }

    if (hasLegalDirection) {
        frame.latestLegalDirection = nextDirection;
    }

    return frame;
}

bool InputSystem::isDirectionKey(int key) const {
    return key == 'w' || key == 'W' || key == 'a' || key == 'A' || key == 's' || key == 'S' || key == 'd' ||
           key == 'D' || key == 256 + 72 || key == 256 + 77 || key == 256 + 80 || key == 256 + 75;
}

std::optional<Direction> InputSystem::toDirection(int key) const {
    if (key == 'w' || key == 'W' || key == 256 + 72) {
        return Direction::Up;
    }
    if (key == 'd' || key == 'D' || key == 256 + 77) {
        return Direction::Right;
    }
    if (key == 's' || key == 'S' || key == 256 + 80) {
        return Direction::Down;
    }
    if (key == 'a' || key == 'A' || key == 256 + 75) {
        return Direction::Left;
    }
    return std::nullopt;
}

int InputSystem::readSingleKey() const {
    int key = _getch();
    if (key == 0 || key == 224) {
        key = 256 + _getch();
    }
    return key;
}
