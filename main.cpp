#include <ctime>
#include <cstdlib>
#include <algorithm>

#define NOMINMAX
#include <Windows.h>

#include "core/GameLoop.h"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;

    int boardWidth = 40;
    int boardHeight = 24;
    int screenWidth = 76;
    constexpr int panelWidth = 34;

    if (GetConsoleScreenBufferInfo(hConsole, &info)) {
        const int windowWidth = info.srWindow.Right - info.srWindow.Left + 1;
        const int windowHeight = info.srWindow.Bottom - info.srWindow.Top + 1;

        screenWidth = std::max(76, windowWidth);
        boardWidth = std::max(30, screenWidth - panelWidth);
        boardHeight = std::max(15, windowHeight - 3);
    }

    GameLoop gameLoop(boardWidth, boardHeight, screenWidth);
    gameLoop.run();

    return 0;
}
