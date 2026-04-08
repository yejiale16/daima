#include "Game.h"

#include <chrono>
#include <cstdlib>
#include <deque>
#include <thread>

Game::Game(int width, int height)
    : width(width),
      height(height),
      running(true),
      score(0),
      status(GameStatus::Start),
      renderer(width, height),
      saveSystem("savegame.txt") {
    resetGame();
    status = GameStatus::Start;
}

void Game::run() {
    using clock = std::chrono::steady_clock;
    auto lastStep = clock::now();
    constexpr auto stepInterval = std::chrono::milliseconds(120);

    while (running) {
        InputCommand cmd = input.poll();
        processInput(cmd);

        const auto now = clock::now();
        if (status == GameStatus::Running && now - lastStep >= stepInterval) {
            updateLogic();
            lastStep = now;
        }

        renderer.render(snake, food, score, status, buildInfoText());
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Game::resetGame() {
    const int centerX = width / 2;
    const int centerY = height / 2;
    snake.reset(centerX, centerY);
    food.generate(width, height, snake.getBody());
    score = 0;
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

std::string Game::buildInfoText() const {
    std::string common = "WASD/Arrows: Move  P: Pause  K: Save  L: Load  Q: Quit";

    if (status == GameStatus::Start) {
        return "SPACE to start. " + common;
    }
    if (status == GameStatus::Paused) {
        return "Paused. Press P to continue. " + common;
    }
    if (status == GameStatus::GameOver) {
        return "Game Over. Press SPACE to restart. " + common;
    }
    return common;
}

bool Game::isWallCollision(const Point& head) const {
    return head.x <= 0 || head.x >= width - 1 || head.y <= 0 || head.y >= height - 1;
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
