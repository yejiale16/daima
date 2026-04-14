#include "Game.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <deque>
#include <thread>

Game::Game(int width, int height, int screenWidth)
    : width(width),
      height(height),
      running(true),
      score(0),
      status(GameStatus::Start),
            renderer(width, height, screenWidth),
            saveSystem("savegame.txt"),
            currentDifficulty(1),
            currentSpeedMs(180),
            currentSnakeSpeed(1000.0 / 180.0) {
    resetGame();
    status = GameStatus::Start;
}

void Game::run() {
    using clock = std::chrono::steady_clock;
    auto lastStep = clock::now();

    while (running) {
        InputCommand cmd = input.poll();
        processInput(cmd);

        currentDifficulty = computeDifficulty();
        currentSpeedMs = computeStepIntervalMs(currentDifficulty);
        currentSnakeSpeed = computeSnakeSpeed(currentSpeedMs);
        const auto stepInterval = std::chrono::milliseconds(currentSpeedMs);

        const auto now = clock::now();
        if (status == GameStatus::Running && now - lastStep >= stepInterval) {
            updateLogic();
            lastStep = now;
        }

        renderer.render(snake, food, score, currentDifficulty, currentSnakeSpeed, status, buildInfoText());
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Game::resetGame() {
    const int centerX = width / 2;
    const int centerY = height / 2;
    snake.reset(centerX, centerY);
    food.generate(width, height, snake.getBody());
    score = 0;
    currentDifficulty = 1;
    currentSpeedMs = 180;
    currentSnakeSpeed = computeSnakeSpeed(currentSpeedMs);
}

void Game::processInput(const InputCommand& cmd) {
    if (cmd.quit) {
        running = false;
        return;
    }

    if (cmd.start) {
        if (status == GameStatus::Start) {
            status = GameStatus::Running;
        } else if (status == GameStatus::GameOver) {
            resetGame();
            status = GameStatus::Running;
        }
    }

    if (cmd.pauseToggle) {
        if (status == GameStatus::Running) {
            status = GameStatus::Paused;
        } else if (status == GameStatus::Paused) {
            status = GameStatus::Running;
        }
    }

    if (cmd.direction.has_value()) {
        snake.setDirection(*cmd.direction);
    }

    if (cmd.save) {
        saveSystem.save(captureSaveData());
    }

    if (cmd.load) {
        if (restoreFromSave()) {
            status = GameStatus::Paused;
        }
    }
}

void Game::updateLogic() {
    const Point head = snake.getHead();
    Point next = head;

    switch (snake.getDirection()) {
        case Direction::Up:
            --next.y;
            break;
        case Direction::Right:
            ++next.x;
            break;
        case Direction::Down:
            ++next.y;
            break;
        case Direction::Left:
            --next.x;
            break;
    }

    if (isWallCollision(next) || snake.occupies(next)) {
        status = GameStatus::GameOver;
        return;
    }

    const bool grow = (next == food.getPosition());
    snake.move(grow);

    if (snake.isSelfCollision()) {
        status = GameStatus::GameOver;
        return;
    }

    if (grow) {
        score += 10;
        food.generate(width, height, snake.getBody());
    }
}

std::wstring Game::buildInfoText() const {
    if (status == GameStatus::Start) {
        return L"\u6309\u7a7a\u683c\u5f00\u59cb\u6e38\u620f\u3002";
    }
    if (status == GameStatus::Paused) {
        return L"\u5df2\u6682\u505c\u3002\u6309 P \u7ee7\u7eed\u3002";
    }
    if (status == GameStatus::GameOver) {
        return L"\u6e38\u620f\u7ed3\u675f\u3002\u6309\u7a7a\u683c\u91cd\u65b0\u5f00\u59cb\u3002";
    }
    return L"\u8fdb\u884c\u4e2d\u3002";
}

bool Game::isWallCollision(const Point& head) const {
    return head.x <= 0 || head.x >= width - 1 || head.y <= 0 || head.y >= height - 1;
}

int Game::computeDifficulty() const {
    const int level = 1 + (score / 50);
    return std::clamp(level, 1, 9);
}

int Game::computeStepIntervalMs(int difficulty) const {
    const int interval = 180 - (difficulty - 1) * 15;
    return (std::max)(60, interval);
}

double Game::computeSnakeSpeed(int stepIntervalMs) const {
    if (stepIntervalMs <= 0) {
        return 0.0;
    }
    return 1000.0 / static_cast<double>(stepIntervalMs);
}

SavedGameData Game::captureSaveData() const {
    SavedGameData data;
    data.direction = static_cast<int>(snake.getDirection());
    data.food = food.getPosition();
    data.score = score;
    data.status = static_cast<int>(status);
    data.snake.assign(snake.getBody().begin(), snake.getBody().end());
    return data;
}

bool Game::restoreFromSave() {
    SavedGameData data;
    if (!saveSystem.load(data)) {
        return false;
    }

    std::deque<Point> restoredBody(data.snake.begin(), data.snake.end());
    if (restoredBody.empty()) {
        return false;
    }

    snake.setBody(restoredBody);
    snake.setDirectionDirect(static_cast<Direction>(data.direction));
    food.setPosition(data.food);
    score = data.score;
    status = static_cast<GameStatus>(data.status);
    return true;
}
