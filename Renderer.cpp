#include "Renderer.h"

Renderer::Renderer(int width, int height)
    : width(width),
      height(height),
      screenHeight(height + 3),
      hConsole(GetStdHandle(STD_OUTPUT_HANDLE)),
      frontBuffer(width * (height + 3)),
      backBuffer(width * (height + 3)),
      useFront(true) {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void Renderer::render(const Snake& snake, const Food& food, int score, GameStatus status, const std::string& infoText) {
    std::vector<CHAR_INFO>& drawBuffer = useFront ? backBuffer : frontBuffer;
    clear(drawBuffer);

    for (int x = 0; x < width; ++x) {
        putChar(drawBuffer, x, 0, '#', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        putChar(drawBuffer, x, height - 1, '#', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
    for (int y = 0; y < height; ++y) {
        putChar(drawBuffer, 0, y, '#', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        putChar(drawBuffer, width - 1, y, '#', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }

    const auto& body = snake.getBody();
    for (size_t i = 0; i < body.size(); ++i) {
        const Point& p = body[i];
        if (i == 0) {
            putChar(drawBuffer, p.x, p.y, 'O', FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        } else {
            putChar(drawBuffer, p.x, p.y, 'o', FOREGROUND_GREEN);
        }
    }

    Point fp = food.getPosition();
    putChar(drawBuffer, fp.x, fp.y, 'F', FOREGROUND_RED | FOREGROUND_INTENSITY);

    putText(drawBuffer, 0, height, "Score: " + std::to_string(score), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    std::string statusText = "State: ";
    switch (status) {
        case GameStatus::Start:
            statusText += "Start";
            break;
        case GameStatus::Running:
            statusText += "Running";
            break;
        case GameStatus::Paused:
            statusText += "Paused";
            break;
        case GameStatus::GameOver:
            statusText += "GameOver";
            break;
    }
    putText(drawBuffer, 16, height, statusText, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    putText(drawBuffer, 0, height + 1, infoText, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    SMALL_RECT rect = {0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(screenHeight - 1)};
    COORD bufferSize = {static_cast<SHORT>(width), static_cast<SHORT>(screenHeight)};
    COORD bufferCoord = {0, 0};

    WriteConsoleOutputA(hConsole, drawBuffer.data(), bufferSize, bufferCoord, &rect);
    useFront = !useFront;
}

void Renderer::clear(std::vector<CHAR_INFO>& buffer) {
    for (CHAR_INFO& c : buffer) {
        c.Char.AsciiChar = ' ';
        c.Attributes = 0;
    }
}

void Renderer::putChar(std::vector<CHAR_INFO>& buffer, int x, int y, char ch, WORD attr) {
    if (x < 0 || x >= width || y < 0 || y >= screenHeight) {
        return;
    }

    CHAR_INFO& cell = buffer[y * width + x];
    cell.Char.AsciiChar = ch;
    cell.Attributes = attr;
}

void Renderer::putText(std::vector<CHAR_INFO>& buffer, int x, int y, const std::string& text, WORD attr) {
    for (size_t i = 0; i < text.size(); ++i) {
        putChar(buffer, x + static_cast<int>(i), y, text[i], attr);
    }
}
