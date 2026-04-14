#pragma once

#include <string>
#include <vector>

#include <Windows.h>

#include "core/StateMachine.h"
#include "gameplay/Food.h"
#include "gameplay/Snake.h"

class RenderSystem {
public:
    RenderSystem(int boardWidth, int boardHeight, int screenWidth);

    void render(const Snake& snake,
                const Food& food,
                int score,
                int difficulty,
                double snakeSpeed,
                GameState state,
                const std::wstring& statusText);

private:
    int boardWidth;
    int boardHeight;
    int screenWidth;
    int screenHeight;
    int panelStartX;

    HANDLE hConsole;
    std::vector<CHAR_INFO> frontBuffer;
    std::vector<CHAR_INFO> backBuffer;
    bool useFront;

    void clear(std::vector<CHAR_INFO>& buffer);
    void putChar(std::vector<CHAR_INFO>& buffer, int x, int y, wchar_t ch, WORD attr);
    void putText(std::vector<CHAR_INFO>& buffer, int x, int y, const std::wstring& text, WORD attr);
};
