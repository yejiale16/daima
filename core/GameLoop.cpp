#include "core/GameLoop.h"

#include <chrono>
#include <deque>
#include <thread>

GameLoop::GameLoop(int boardWidth, int boardHeight, int screenWidth)
    : width(boardWidth),
      height(boardHeight),
      saveSystem("savegame.txt"),
      renderSystem(boardWidth, boardHeight, screenWidth),
      score(0) {
    resetGame();
    fsm.setState(GameState::MENU);
}

void GameLoop::run() {
    while (fsm.state() != GameState::EXIT) {
        processInput();

        if (fsm.state() == GameState::RUNNING) {
            const int steps = timeSystem.consumePendingSteps();
            for (int i = 0; i < steps; ++i) {
                update();
            }
        }

        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void GameLoop::processInput() {
    const InputFrame frame = inputSystem.processInput(snake.direction(), fsm.state());

    if (frame.quit) {
        fsm.requestExit();
        return;
    }

    if (frame.start) {
        if (fsm.state() == GameState::MENU || fsm.state() == GameState::GAME_OVER) {
            resetGame();
            fsm.requestStart();
        }
    }

    if (frame.pauseToggle) {
        fsm.requestPauseToggle();
    }

    if (frame.save) {
        saveSystem.save(buildSaveData());
    }

    if (frame.load) {
        restoreFromSave();
    }

    if (fsm.state() == GameState::RUNNING && frame.latestLegalDirection.has_value()) {
        snake.setDirection(*frame.latestLegalDirection);
    }
}

void GameLoop::update() {
    timeSystem.updateDifficultyByScore(score);

    Point next = snake.head();
    switch (snake.direction()) {
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
        fsm.requestGameOver();
        return;
    }

    const bool grow = (next == food.position());
    snake.move(grow);

    if (grow) {
        score += 10;
        timeSystem.updateDifficultyByScore(score);
        food.generate(width, height, snake.body());
    }
}

void GameLoop::render() {
    renderSystem.render(
        snake,
        food,
        score,
        timeSystem.difficulty(),
        timeSystem.snakeSpeedCellsPerSec(),
        fsm.state(),
        statusLine());
}

void GameLoop::resetGame() {
    const int centerX = width / 2;
    const int centerY = height / 2;
    snake.reset(centerX, centerY);
    food.generate(width, height, snake.body());
    score = 0;
    timeSystem.updateDifficultyByScore(score);
    timeSystem.reset();
}

bool GameLoop::isWallCollision(const Point& head) const {
    return head.x <= 0 || head.x >= width - 1 || head.y <= 0 || head.y >= height - 1;
}

std::wstring GameLoop::statusLine() const {
    switch (fsm.state()) {
        case GameState::MENU:
            return L"\u6e38\u620f\u72b6\u6001: \u83dc\u5355  (Space \u5f00\u59cb)";
        case GameState::RUNNING:
            return L"\u6e38\u620f\u72b6\u6001: \u8fdb\u884c\u4e2d";
        case GameState::PAUSED:
            return L"\u6e38\u620f\u72b6\u6001: \u6682\u505c  (P \u7ee7\u7eed)";
        case GameState::GAME_OVER:
            return L"\u6e38\u620f\u72b6\u6001: \u7ed3\u675f  (Space \u91cd\u5f00)";
        case GameState::EXIT:
            return L"\u6e38\u620f\u72b6\u6001: \u9000\u51fa";
    }
    return L"";
}

SaveData GameLoop::buildSaveData() const {
    SaveData data;
    data.snake.assign(snake.body().begin(), snake.body().end());
    data.food = food.position();
    data.direction = static_cast<int>(snake.direction());
    data.score = score;
    data.gameState = static_cast<int>(fsm.state());
    return data;
}

bool GameLoop::restoreFromSave() {
    SaveData data;
    if (!saveSystem.load(data)) {
        return false;
    }

    std::deque<Point> body(data.snake.begin(), data.snake.end());
    if (body.empty()) {
        return false;
    }

    snake.setBody(body);
    snake.setDirectionDirect(static_cast<Direction>(data.direction));
    food.setPosition(data.food);
    score = data.score;

    GameState loadedState = static_cast<GameState>(data.gameState);
    if (loadedState == GameState::EXIT) {
        loadedState = GameState::MENU;
    }
    fsm.setState(loadedState);

    timeSystem.updateDifficultyByScore(score);
    timeSystem.reset();
    return true;
}
