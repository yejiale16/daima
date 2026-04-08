#include "Input.h"

#include <Windows.h>

Input::Input() {
    previousState.fill(false);
}

InputCommand Input::poll() {
    InputCommand cmd;

    if (pressedOnce('W') || pressedOnce(VK_UP)) {
        cmd.direction = Direction::Up;
    } else if (pressedOnce('D') || pressedOnce(VK_RIGHT)) {
        cmd.direction = Direction::Right;
    } else if (pressedOnce('S') || pressedOnce(VK_DOWN)) {
        cmd.direction = Direction::Down;
    } else if (pressedOnce('A') || pressedOnce(VK_LEFT)) {
        cmd.direction = Direction::Left;
    }

    cmd.quit = pressedOnce('Q') || pressedOnce(VK_ESCAPE);
    cmd.pauseToggle = pressedOnce('P');
    cmd.save = pressedOnce('K');
    cmd.load = pressedOnce('L');
    cmd.start = pressedOnce(VK_SPACE);

    return cmd;
}

bool Input::pressedOnce(int vk) {
    const bool isDown = (GetAsyncKeyState(vk) & 0x8000) != 0;
    const bool once = isDown && !previousState[vk];
    previousState[vk] = isDown;
    return once;
}
