#pragma once

#include <string>
#include <vector>

#include <Windows.h>

#include "Food.h"
#include "Snake.h"

enum class GameStatus {
    Start = 0,
    Running = 1,
    Paused = 2,
    GameOver = 3
};

class Renderer {
public:
    Renderer(int width, int height);

    void render(const Snake& snake, const Food& food, int score, GameStatus status, const std::string& infoText);

private:
    int width;
    int height;
    int screenHeight;

    HANDLE hConsole;
    std::vector<CHAR_INFO> frontBuffer;
    std::vector<CHAR_INFO> backBuffer;
    bool useFront;

    void clear(std::vector<CHAR_INFO>& buffer);
    void putChar(std::vector<CHAR_INFO>& buffer, int x, int y, char ch, WORD attr);
    void putText(std::vector<CHAR_INFO>& buffer, int x, int y, const std::string& text, WORD attr);
};
