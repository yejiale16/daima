#pragma once

class TimeSystem {
public:
    TimeSystem();

    void reset();
    void updateDifficultyByScore(int score);
    int consumePendingSteps();

    int difficulty() const;
    int tickIntervalMs() const;
    double snakeSpeedCellsPerSec() const;

private:
    int difficultyLevel;
    int intervalMs;
    long long accumulatorStartMs;

    long long nowMs() const;
};
