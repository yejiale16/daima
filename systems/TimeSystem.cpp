#include "systems/TimeSystem.h"

#include <algorithm>
#include <chrono>

TimeSystem::TimeSystem() : difficultyLevel(1), intervalMs(180), accumulatorStartMs(nowMs()) {
}

void TimeSystem::reset() {
    accumulatorStartMs = nowMs();
}

void TimeSystem::updateDifficultyByScore(int score) {
    const int raw = 1 + score / 50;
    difficultyLevel = std::clamp(raw, 1, 9);
    const int base = 180 - (difficultyLevel - 1) * 15;
    intervalMs = (std::max)(60, base);
}

int TimeSystem::consumePendingSteps() {
    const long long now = nowMs();
    int steps = 0;

    while (now - accumulatorStartMs >= intervalMs) {
        accumulatorStartMs += intervalMs;
        ++steps;
        if (steps >= 5) {
            break;
        }
    }

    return steps;
}

int TimeSystem::difficulty() const {
    return difficultyLevel;
}

int TimeSystem::tickIntervalMs() const {
    return intervalMs;
}

double TimeSystem::snakeSpeedCellsPerSec() const {
    if (intervalMs <= 0) {
        return 0.0;
    }
    return 1000.0 / static_cast<double>(intervalMs);
}

long long TimeSystem::nowMs() const {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}
