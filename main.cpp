#include <ctime>
#include <cstdlib>
#include <algorithm>

#define NOMINMAX
#include <Windows.h>

#include "Game.h"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;

    int boardWidth = 40;
    int boardHeight = 24;

    if (GetConsoleScreenBufferInfo(hConsole, &info)) {
        const int windowWidth = info.srWindow.Right - info.srWindow.Left + 1;
        const int windowHeight = info.srWindow.Bottom - info.srWindow.Top + 1;

        boardWidth = std::max(30, windowWidth);
        boardHeight = std::max(15, windowHeight - 3);
    }

    Game game(boardWidth, boardHeight);
    game.run();

    return 0;
}
