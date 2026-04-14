#include "systems/RenderSystem.h"

#include <iomanip>
#include <sstream>

RenderSystem::RenderSystem(int boardWidth, int boardHeight, int screenWidth)
    : boardWidth(boardWidth),
      boardHeight(boardHeight),
      screenWidth(screenWidth),
      screenHeight(boardHeight + 3),
      panelStartX(boardWidth + 2),
      hConsole(GetStdHandle(STD_OUTPUT_HANDLE)),
      frontBuffer(screenWidth * (boardHeight + 3)),
      backBuffer(screenWidth * (boardHeight + 3)),
      useFront(true) {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void RenderSystem::render(const Snake& snake,
                          const Food& food,
                          int score,
                          int difficulty,
                          double snakeSpeed,
                          GameState state,
                          const std::wstring& statusText) {
    std::vector<CHAR_INFO>& drawBuffer = useFront ? backBuffer : frontBuffer;
    clear(drawBuffer);

    const WORD borderAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    const WORD snakeAttr = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    const WORD headAttr = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    const WORD foodAttr = FOREGROUND_RED | FOREGROUND_INTENSITY;
    const WORD panelAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    for (int x = 0; x < boardWidth; ++x) {
        putChar(drawBuffer, x, 0, L'\u25A1', borderAttr);
        putChar(drawBuffer, x, boardHeight - 1, L'\u25A1', borderAttr);
    }
    for (int y = 0; y < boardHeight; ++y) {
        putChar(drawBuffer, 0, y, L'\u25A1', borderAttr);
        putChar(drawBuffer, boardWidth - 1, y, L'\u25A1', borderAttr);
    }

    const auto& body = snake.body();
    for (size_t i = 0; i < body.size(); ++i) {
        const Point& p = body[i];
        if (i == 0) {
            putChar(drawBuffer, p.x, p.y, L'\u25CF', headAttr);
        } else {
            putChar(drawBuffer, p.x, p.y, L'\u25CB', snakeAttr);
        }
    }

    const Point foodPosition = food.position();
    putChar(drawBuffer, foodPosition.x, foodPosition.y, L'\u2605', foodAttr);

    putText(drawBuffer, 0, boardHeight, L"\u6e38\u620f\u5f97\u5206: " + std::to_wstring(score), panelAttr);
    putText(drawBuffer, 14, boardHeight, statusText, panelAttr);

    putText(drawBuffer, panelStartX, 1, L"\u6e38\u620f\u8bf4\u660e", panelAttr);
    putText(drawBuffer, panelStartX, 2, L"A. \u86c7\u649e\u5230\u5899\u6216\u81ea\u8eab\uff0c\u6e38\u620f\u7ed3\u675f", panelAttr);
    putText(drawBuffer, panelStartX, 3, L"B. \u6e38\u620f\u96be\u5ea6\u4f1a\u968f\u5206\u6570\u589e\u52a0\u800c\u63d0\u5347", panelAttr);

    putText(drawBuffer, panelStartX, 5, L"\u64cd\u4f5c\u8bf4\u660e", panelAttr);
    putText(drawBuffer, panelStartX, 6, L"\u25a1 \u5411\u5de6\u79fb\u52a8: \u2190 / A", panelAttr);
    putText(drawBuffer, panelStartX, 7, L"\u25a1 \u5411\u53f3\u79fb\u52a8: \u2192 / D", panelAttr);
    putText(drawBuffer, panelStartX, 8, L"\u25a1 \u5411\u4e0b\u79fb\u52a8: \u2193 / S", panelAttr);
    putText(drawBuffer, panelStartX, 9, L"\u25a1 \u5411\u4e0a\u79fb\u52a8: \u2191 / W", panelAttr);
    putText(drawBuffer, panelStartX, 10, L"\u25a1 \u5f00\u59cb/\u91cd\u5f00: Space", panelAttr);
    putText(drawBuffer, panelStartX, 11, L"\u25a1 \u6682\u505c\u7ee7\u7eed: P", panelAttr);
    putText(drawBuffer, panelStartX, 12, L"\u25a1 \u5b58\u6863/\u8bfb\u6863: K / L", panelAttr);
    putText(drawBuffer, panelStartX, 13, L"\u25a1 \u9000\u51fa\u6e38\u620f: Q / Esc", panelAttr);

    putText(drawBuffer, panelStartX, 15, L"\u4f5c\u8005\uff1a\u676d\u7535\u6570\u5a92  \u53f6\u5609\u4e50", panelAttr);
    putText(drawBuffer, panelStartX, 16, L"\u2606 \u5f53\u524d\u96be\u5ea6: " + std::to_wstring(difficulty) + L" \u7ea7", panelAttr);
    putText(drawBuffer, panelStartX, 18, L"\u25cf \u5f53\u524d\u5f97\u5206: " + std::to_wstring(score), panelAttr);

    std::wostringstream speed;
    speed << std::fixed << std::setprecision(1) << snakeSpeed;
    putText(drawBuffer, panelStartX, 19, L"\u25cf \u5f53\u524d\u86c7\u901f\u5ea6: " + speed.str() + L" \u683c/\u79d2", panelAttr);

    SMALL_RECT rect = {0, 0, static_cast<SHORT>(screenWidth - 1), static_cast<SHORT>(screenHeight - 1)};
    COORD bufferSize = {static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight)};
    COORD startCoord = {0, 0};

    WriteConsoleOutputW(hConsole, drawBuffer.data(), bufferSize, startCoord, &rect);
    useFront = !useFront;
}

void RenderSystem::clear(std::vector<CHAR_INFO>& buffer) {
    for (CHAR_INFO& c : buffer) {
        c.Char.UnicodeChar = L' ';
        c.Attributes = 0;
    }
}

void RenderSystem::putChar(std::vector<CHAR_INFO>& buffer, int x, int y, wchar_t ch, WORD attr) {
    if (x < 0 || x >= screenWidth || y < 0 || y >= screenHeight) {
        return;
    }

    CHAR_INFO& cell = buffer[y * screenWidth + x];
    cell.Char.UnicodeChar = ch;
    cell.Attributes = attr;
}

void RenderSystem::putText(std::vector<CHAR_INFO>& buffer, int x, int y, const std::wstring& text, WORD attr) {
    for (size_t i = 0; i < text.size(); ++i) {
        putChar(buffer, x + static_cast<int>(i), y, text[i], attr);
    }
}
